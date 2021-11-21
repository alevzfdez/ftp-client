
/*#######################################################################################################*/
/*#######################################################################################################*/
/*#######                                                                                         #######*/
/*#######                                                                                         #######*/
/*#######                  Code made by Alejandro Véliz Fernández et Jorge Sendino                #######*/
/*#######                                                                                         #######*/
/*#######                       Laboratorio de Redes, Servicios y Sistemas                        #######*/
/*#######                                 Bloque 1 - FTP client                                   #######*/
/*#######                                                                                         #######*/
/*#######################################################################################################*/
/*#######################################################################################################*/

#include "cFTP.h"

/* FUNCTION DECLARATIONS */
void header()
{
   printf("\n\n\e[1;33m   ________  ________  _______                        ______   __        ______  ________  __    __  ________\n"
                     "  /        |/        |/       \\                      /      \\ /  |      /      |/        |/  \\  /  |/        |\n"
                     "  $$$$$$$$/ $$$$$$$$/ $$$$$$$  |                    /$$$$$$  |$$ |      $$$$$$/ $$$$$$$$/ $$  \\ $$ |$$$$$$$$/ \n"
                     "  $$ |__       $$ |   $$ |__$$ |       ______       $$ |  $$/ $$ |        $$ |  $$ |__    $$$  \\$$ |   $$ |\n"
                     "  $$    |      $$ |   $$    $$/       /      |      $$ |      $$ |        $$ |  $$    |   $$$$  $$ |   $$ |\n"
                     "  $$$$$/       $$ |   $$$$$$$/        $$$$$$/       $$ |   __ $$ |        $$ |  $$$$$/    $$ $$ $$ |   $$ |\n"
                     "  $$ |         $$ |   $$ |                          $$ \\__/  |$$ |_____  _$$ |_ $$ |_____ $$ |$$$$ |   $$ |\n"
                     "  $$ |         $$ |   $$ |                          $$    $$/ $$       |/ $$   |$$       |$$ | $$$ |   $$ |\n"
                     "  $$/          $$/    $$/                            $$$$$$/  $$$$$$$$/ $$$$$$/ $$$$$$$$/ $$/   $$/    $$/ \n\n");

}

int optChosen(int argcpt,char *argvpt[])
{
  int opt = 0, i, flags[4]={0};

  while ((opt = getopt(argcpt, argvpt, "vs")) != -1)
  {
    switch (opt)
    {
      case 'v':
        flags[3] = 10;
        verbose = 1;
        break;
       case 's':
         flags[3] = 1;
         silent = 0;
         break;
      default:
        optErr();
        break;
    }
  } opt = 0;

  for (i = 0; i < 4; i++)
    if (flags[i] != 0)
      opt += flags[i];

  // check if arguments are ok
  checkUsage(argcpt, argvpt, opt);

  return opt;
}

void checkUsage(int argcpt,char *argvpt[], int opt)
{
    if (opt == 10 && argcpt != 3)
      optErr();
    else if (opt == 0 && argcpt != 2)
      optErr();
   if (opt == 10 || opt == 11)
      verbose = 1;
   else
      verbose = 0;
   if(opt == 11 || opt == 1)
      silent = 0;
   else
      silent = 1;
}

void optErrcmd()
{
   printf("\e[0;37mCommands accepted:\n"
            "\t- pwd\t\t\t show current directory\n"
            "\t- ls\t\t\t list current directory\n"
            "\t- cd <directory>\t move to directory\n"
            "\t- size <file>\t\t get file size\n"
            "\t- get <file>\t\t download file\n"
            "\t- put <file>\t\t upload file\n"
            "\t- clear\t\t\t clean screen\n"
            "\t- help\t\t\t show this information\n"
            "\t- bye\t\t\t close program\n\n"
            "Note that short commands are allowed,\n"
            "\t- p\t equals to\t pwd\n"
            "\t- l\t equals to\t ls\n"
            "\t- cd\t equals to\t cd\n"
            "\t- s\t equals to\t size\n"
            "\t- g\t equals to\t get\n"
            "\t- pu\t equals to\t put\n"
            "\t- c\t equals to\t clear\n"
            "\t- h\t equals to\t help\n"
            "\t- b\t equals to\t bye\n\n"
            "*Uppercase commands allowed\n\n");
}

void optErr()
{
    fprintf(stderr, "Error!\t");
    printf("Usage: ./client_FTP [-v verbose] [-s silent mode] <servidorFTP> \n"
            "Note,\n"
            "   - verbose and silent modes are optionall\n"
            "   - verbose mode show mora info data in case you need\n"
            "   - silent mode only show most relevant data making a cleaner interface\n");

    exit(1);
}

void sysError(const char *msg)
{
    perror(msg);
    exit(1);
}

void closeAll(int a)                                  // close socket indicating both sending and receiving are not allowed
{
   if (newSocketHandler != 0)
      shutdown(newSocketHandler, 2);
   shutdown(socketHandler, 2);

    if (verbose)
        printf("\nSockets closed correctly.");

  printf("\e[0;37m");
   exit (1);
}

//////////////////////////////  SOCKETS UTILS  //////////////////////////////

void operationWrite(int sockt, char *arv)                                                 // write string given into socket given
{
      //   write data
      if (write(sockt, arv, strlen(arv)) < 0) {
        fprintf(stderr, "Error! writing socket data fails!!\n");
        closeAll(1);
      } if (verbose) {
         printf("\tRAW command sent: %s\n", arv);
         printf("\twaiting for response...\n\n");
      }
}

char* operationRead(int flag, int sockt, int buff_len, char filename[50])
{
   int i, j, run = 1, fileFlag=0, flagOpened = 0, n=0, buff_leee=buff_len;
   char buffer[MAX_BUFF];
   FILE *stream;
   fd_set fds;                      // file descriptor set
   struct timeval tv;             // timeval struct
           struct timeval t1, t2;
           long int timediff_s, timediff_us;

   // set timeout to 1,5s
   tv.tv_sec = 10;
   tv.tv_usec = 0;

   FD_ZERO(&fds);
   FD_SET(sockt, &fds);

 gettimeofday(&t1, NULL);
   do
   {
      // clear buffer
      bzero(buffer, MAX_BUFF);                                    // clear buffer
      fflush(stdout);

      select(sockt+1, &fds, NULL, NULL, &tv);

      // for(i = 1; i < sockt; i++)
      // {

         if (FD_ISSET(sockt, &fds)) {
           // read
           if ((n=read(sockt, buffer, MAX_BUFF)) < 0) {
             fprintf(stderr, "Error! Reading socket data fails!!\n");
             closeAll(1);
          }


            if (strcmp(filename, " ") != 0 && flagOpened == 0)
              fileFlag = 1;

            switch(fileFlag)
            {
              case 0:
               // check if response is multiline, then check if current character of buffer is an integer, if there is a '-' following,
               // if there's not a '-' is the last response line, else we return to reading statement
                if (buffer[3] == '-') {
                  for (i = 0; i < MAX_BUFF; i++)
                     if(isdigit(buffer[i])) {
                        if (buffer[i+3] != '-') {
                           run = 0;
                           break;
                        } i += 3;
                     }
               } else
                  run = 0;
              break;
              case 1:
                stream = fopen(filename, "wb+");
               //  stream = open(filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
                flagOpened = 1;
                printf("\nDownloading data:\n");
               //  write(stream, buffer, n);
                if (buff_len > MAX_BUFF)
                  fileFlag = 2;
               else {
                  fileFlag = 3;
                  buff_len = 0;
               }
                  // write(stream, buffer, n);
                fwrite(buffer, sizeof(buffer), 1, stream);
              break;
              case 2:
                fileFlag = 2;
               //  write(stream, buffer, n);
                fwrite(buffer, sizeof(buffer), 1, stream);
                buff_len -= MAX_BUFF;
                if (buff_len <= 0) {
                   buff_len = 0;
                  fileFlag = 3;
                } else{
                  printf("\r\e[0;35m[%3d bytes remaining] \t\t", buff_len);
               }
              break;
              case 3:
                  if (verbose)
                      printf("there's no more data to read from socket ;)\n");

                    printf("\r\e[0;35m[%3d bytes remaining] \t\t\n", buff_len);
                    printf("\nDownload complete!\n");
                  gettimeofday(&t2, NULL);

                     timediff_us = t2.tv_usec - t1.tv_usec;
                     timediff_s = t2.tv_sec - t1.tv_sec;
                    printf("Bytes received:\t%i Bytes \nTime elapsed:\t%ld.%06lds\n", buff_leee, timediff_s, timediff_us);
                    printf("Download rate:\t%Lf MB/s\n", (buff_leee*0.000001L)/(timediff_s + timediff_us*0.000001L));
                   FD_CLR(sockt, &fds);
                  //  close(stream);
                   fclose(stream);

                    run = 0;
              break;
            }
         } else {
            if (verbose)
              printf("there's no more data to read from socket ;)\n");
            if (fileFlag == 2)
            {
               if (buff_len <= 0)
                  buff_len = 0;
               else
                  fwrite(buffer, sizeof(buffer), 1, stream);

              printf("\r\e[0;35m[%3d bytes remaining] \t\t\n", buff_len);
              printf("\nDownload complete!\n");
              FD_CLR(sockt, &fds);
            //   close(stream);
              fclose(stream);
           }
            run = 0;
         }

         if(flag)
            printf("\e[0;37m%s\n", buffer);

       //   else if (fileFlag == 2) {
       //      buff_len -= MAX_BUFF;
       //      percnt = (buff_len/MAX_BUFF);
       //      if ((len = lenHelper(percnt)) > 1){
       //         for (i = 0; i < len-1; i++)
       //          sizediv *= 10;
       //      } else
       //          sizediv = 10;
       //      for (i = 0; i < percnt/sizediv; i++)
       //      {
       //         printf("*");
       //      }
       //   }
    } while(run);
   // }
  return buffer;
}

int makeSocket(int portNo, int argcpt, char *argvpt[])                        // simply create, and connect new tcp socket
{
   int sockt;
   struct sockaddr_in serv_addr;
   struct hostent *server;

   // 1º step, making new socket
   if ((sockt = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      fprintf(stderr, "Error! Opening socket fails!!\n");
      closeAll(1);
   } else
      if (verbose)
           printf("\t1.-  Socket opened correctly\n");

   if ((server = gethostbyname(argvpt[argcpt-1])) == NULL) {
      fprintf(stderr, "unknown host %s\n", argvpt[argcpt-1]);
      closeAll(1);
   } else
      if (verbose)
           printf("\t     Host IP solved ok!!\n");


   /* some stuff after making socket */
   bzero((char *) &serv_addr, sizeof(serv_addr));                                              // set serv_addr to 0's
   serv_addr.sin_family = AF_INET;                                                             // variable to indicate server family is internet protocol address
   bcopy ((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);     // set server IP address
   serv_addr.sin_port = htons(portNo);                                                         // ensure port is set in network byte order

   // 2º step, connect socket
   if (connect(sockt,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
      fprintf(stderr, "Error! Connecting socket fails!!\n");
      closeAll(1);
   } else
      if (verbose)
           printf("\t2.-  Socket connected correctly\n");

   return sockt;
}


                          /*################################  SPECIFIC UTILS  ################################*/

int passMode()
{
   int i, j = 0, portD = 0;
   char buffer[MAX_BUFF]={0};
   char *response, *str;

   operationWrite(socketHandler, "PASV\r\n");
   strcpy(buffer, operationRead(silent, socketHandler, -1, " "));               // set passive mode and copy into buffer

   for(i = 0; i < strlen(buffer); ++i)
      if (buffer[i] == '(') {
         response = &buffer[i];
         response[strlen(response)-3] = '\n';
      }

   while((str = strsep(&response, ",")) != NULL)
   {
      switch(j)
      {
         case 4:
            portD = atoi(str) * 256;
            break;
         case 5:
            portD = portD + atoi(str);
            break;
      } j++;
   }


   if (verbose)
      printf("Connecting to port %d.........\n", portD);

   return portD;
}

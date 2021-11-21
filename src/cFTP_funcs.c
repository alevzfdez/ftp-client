
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

void maker(int opt, int argcpt, char *argvpt[])
{
   int step = 0, run = 1;
   char buffer[MAX_BUFF];
   char str[255], cmd[255], *concat;
   // data vars
   int i, cm = 0, wt_num = 0, buff_len = -1;
   char size[2]={'0'}, *wt[9]= {"clear", "help", "bye", "pwd", "ls", "cd", "size", "get", "put"};
   // get number of options
   wt_num = sizeof(wt)/sizeof(wt[0]);

   // ensure socket is closed when user forze exit
   signal(SIGINT, closeAll);

   if(opt == 10)
      verbose = 1;

   // prints ASCII header
   header();

   while(run)
   {
      switch(step)
      {
         case 0:
            // create new tcp control socket and connect
            socketHandler = makeSocket( 21, argcpt, argvpt);

            // read response from server
            if (read(socketHandler, buffer, MAX_BUFF) < 0) {
                  fprintf(stderr, "\e[0;33mError! Reading socket data fails!!\n");
                  closeAll(1);
            }

            printf("\e[0;37m%s\n", buffer);
            bzero(buffer, MAX_BUFF);
            step = 1;
            break;

         // START CONTROL COMMUNICATION
         case 1:
            // sending user...
            printf("\e[0;32mUser: ");
            fflush(stdin);
            fgets(str, 255, stdin);

            concat = (char *)malloc(strlen(str)+6);

            strcpy(concat, "USER ");
            strncat(concat, str, strlen(str)-1);
            strcat(concat, "\r\n");

            operationWrite( socketHandler, concat);                     // sending user data
            operationRead(silent, socketHandler, -1, " ");
            free(concat);

            // sending pass...
            // printf("Password: ");
            // fflush(stdin);
            // fgets(str, 255, stdin);

            strcpy(str, getpass("\e[0;32mPassword: "));
            concat = (char *)malloc(strlen(str)+6);
            sprintf(concat, "PASS %s\r\n", str);
            operationWrite( socketHandler, concat);                     // sending user data
            operationRead(silent, socketHandler, -1, " ");
            free(concat);

            // get system type
            operationWrite(socketHandler, "SYST\r\n");
            operationRead(silent, socketHandler, -1, " ");
            // get feature list
            operationWrite(socketHandler, "FEAT\r\n");
            operationRead(silent, socketHandler, -1, " ");
            // set binary type
            operationWrite(socketHandler, "TYPE I\r\n");
            operationRead(silent, socketHandler, -1, " ");

            step = 2;
            break;

         // START DATA COMMUNICATION
         case 2:
            // command state machine
            switch(cm)
            {
               case 0:
                  // command line
                  printf("\n\e[0;32mType 'help' if you need.\n");

                  printf("ftp> ");

                  bzero(str, 255);
                  fflush(stdin);
                  gets(cmd);
                  for (i = 0; i < strlen(cmd); i++)
                     if (cmd[i] == ' ') {
                        strncpy(str, &cmd[i+1], strlen(cmd)-(i+1));
                        bzero(&cmd[i], strlen(cmd)-(i+1));
                     }

                  // check option given, shorted answer are allowed
                  for (i = 0; i < wt_num; i++)
                     if(wt[i] && !strncasecmp(cmd, wt[i], strlen(cmd))) {
                        cm = i+1;
                        break;
                     }

                  break;
               case 1:
                  system("clear");
                  header();
                  cm = 0;
                  break;
               case 2:
                  optErrcmd();
                  cm = 0;
                  break;
               case 3:
                  // set binary type
                  operationWrite(socketHandler, "QUIT\r\n");
                  operationRead(silent, socketHandler, -1, " ");
                  printf("\e[0;32mHave a nice day!\n");
                  run = 0;
                  cm = -1;
                  break;
               case 4:
                  operationWrite(socketHandler, "PWD\r\n");
                  operationRead(1, socketHandler, -1, " ");
                  cm = 0;
                  break;
               case 5:
                  // create new tcp control socket and connect
                  newSocketHandler = makeSocket(passMode(), argcpt, argvpt);

                  // set binary type
                  operationWrite(socketHandler, "TYPE I\r\n");
                  operationRead(0, socketHandler, -1, " ");

                  operationWrite(socketHandler, "LIST\r\n");
                  operationRead(1, socketHandler, -1, " ");
                  operationRead(1, newSocketHandler, -1, " ");
                  operationRead(1, socketHandler, -1, " ");
                  close(newSocketHandler);
                  cm = 0;
                  break;
               case 6:
                  concat = (char *)malloc(strlen(str)+6);

                  strcpy(concat, "CWD ");
                  strncat(concat, str, strlen(str));
                  strcat(concat, "\r\n");

                  operationWrite(socketHandler, concat);
                  operationRead(1, socketHandler, -1, " ");

                  free(concat);
                  cm = 0;
                  break;
               case 7:
                  concat = (char *)malloc(strlen(str)+6);

                  strcpy(concat, "SIZE ");
                  strncat(concat, str, strlen(str));
                  strcat(concat, "\r\n");

                  operationWrite(socketHandler, concat);
                  strcpy(buffer, operationRead(0, socketHandler, -1, " "));

                  buff_len = 0;
                  buff_len += atoi(&buffer[4]);
                  if(buff_len < 1000)
                     strcpy(size, "B");
                  else if(buff_len < 1000000)
                     strcpy(size, "kB");
                  else if(buff_len < 1000000000)
                     strcpy(size, "MB");
                  else if(buff_len < 1000000000000)
                     strcpy(size, "GB");
                  else if(buff_len < 1000000000000000)
                     strcpy(size, "TB");

                  printf("\e[0;37mFile size: %d Bytes [file size range: %s]\n\n", buff_len, size);

                  free(concat);
                  cm = 0;
                  break;
               case 8:
                  // create new tcp control socket and connect
                  newSocketHandler = makeSocket(passMode(), argcpt, argvpt);

                  concat = (char *)malloc(strlen(str)+6);

                  strcpy(concat, "SIZE ");
                  strncat(concat, str, strlen(str));
                  strcat(concat, "\r\n");

                  operationWrite(socketHandler, concat);
                  strcpy(buffer, operationRead(0, socketHandler, -1, " "));

                  buff_len = 0;
                  buff_len = atoi(&buffer[4]);
                  printf("%d\n", buff_len);

                  free(concat);

                  // set binary type
                  operationWrite(socketHandler, "TYPE I\r\n");
                  operationRead(0, socketHandler, -1, " ");

                  concat = (char *)malloc(strlen(str)+6);
                  //bzero(buffer);

                  strcpy(concat, "RETR ");
                  strncat(concat, str, strlen(str));
                  strcat(concat, "\r\n");

                  operationWrite(socketHandler, concat);

                  operationRead(silent, socketHandler, -1, " ");

                  operationRead(0, newSocketHandler, buff_len, str);

                  operationRead(1, socketHandler, -1, " ");

                  free(concat);
                  close(newSocketHandler);
                  cm = 0;
                  break;
               case 9:
                  // create new tcp control socket and connect
                  newSocketHandler = makeSocket(passMode(), argcpt, argvpt);

                  concat = (char *)malloc(strlen(str)+6);

                  strcpy(concat, "STOR ");
                  strncat(concat, str, strlen(str)-1);
                  strcat(concat, "\r\n");

                  operationWrite(socketHandler, concat);

                  operationRead(1, socketHandler, -1, " ");
                  operationRead(1, newSocketHandler, buff_len, " ");
                  operationRead(1, newSocketHandler, buff_len, " ");

                  free(concat);
                  close(newSocketHandler);
                  cm = 0;
                  break;
               default:
                  optErrcmd();
                  cm = 0;
                  break;
            }
            break;

         default:
            break;
      }
   }
   //
   // closeAll(1);
}

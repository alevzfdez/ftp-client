
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

int main(int argc, char *argv[])
{
      int opt = 0;
      // system time variables
      struct timeval start, end;
      long seconds;

   	// select proper operation selected
   	opt = optChosen(argc, argv);


      // start timing control
      gettimeofday(&start, NULL);
      // maker
      maker(opt, argc, argv);
      // finish timing control
      gettimeofday(&end, NULL);

      // computing time difference
      seconds  = end.tv_sec  - start.tv_sec;

      printf("\n---------------------------- cFTP data ----------------------------\n"
            "You have been %ld minutes %ld seconds connected to FTP server.\n", seconds/60, seconds);

      exit(0);
}

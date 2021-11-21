
#/*#######################################################################################################*/
#/*#######################################################################################################*/
#/*#######                                                                                         #######*/
#/*#######                                                                                         #######*/
#/*#######                  Code made by Alejandro Véliz Fernández et Jorge Sendino                #######*/
#/*#######                                                                                         #######*/
#/*#######                       Laboratorio de Redes, Servicios y Sistemas                        #######*/
#/*#######                                 Bloque 1 - FTP client                                   #######*/
#/*#######                                                                                         #######*/
#/*#######################################################################################################*/
#/*#######################################################################################################*/

CFLAGS = -g -Wall

all: cFTP.c cFTP_funcs.c utils.c
	rm -f client_FTP
	cc $(CFLAGS) cFTP.c cFTP_funcs.c utils.c -o client_FTP
	@echo "\nExecutable program make succesfully!!"

clean:
	rm -f client_FTP
	@echo "\nExecutable program removed succesfully!!"

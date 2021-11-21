
	/*########################################################################*/
	/*########################################################################*/
	/*#######										     					 																	     				                 #######*/
	/*#######								Code made by Alejandro Véliz Fernández et Jorge Sendino								  #######*/
	/*#######										     					  														  			   									  #######*/
	/*#######										   Laboratorio de Redes, Servicios y Sistemas										      #######*/
	/*#######						                                Bloque 1 - FTP client					                                       #######*/
	/*#######										     					  																		                             #######*/
	/*########################################################################*/
	/*########################################################################*/

#ifndef CFTP_H_
#define CFTP_H_


/* HEADERS INCLUDED */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>                               	// fix bzero() warning
#include <unistd.h>                             	// needed for getopt() and sleep()
#include <signal.h>                               	// user for 'ctrl+c' event capture
#include <sys/time.h>                             // used to get system time
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>					      // socket header needed
#include <netinet/in.h>						   // needed for domains
#include <netdb.h>                              // defines hostent structure
#include <arpa/inet.h>

/* DEFINITIONS */
#define MAX_BUFF  10000
#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)

/////// Global variables ///////
int verbose, silent, socketHandler, newSocketHandler;

/* FUNCTION DEFINITIONS */

/////// Utils ///////

// common utils
void header();													// prints ASCII header
int optChosen(int argcpt,char *argvpt[]);			// select option choosen
void checkUsage(int argcpt,char *argvpt[], int opt);	// check program usage
void optErrcmd();										// show cmd commands admited
void optErr();							                  // error on option calls
void sysError(const char *msg);             // system error
void closeAll(int a);									// close all sockets

// sockets utils
int makeSocket(int portNo,int argcpt, char *argvpt[]);
void operationWrite(int sockt, char *arv);
char* operationRead(int flag, int sockt, int buff_len, char filename[50]);

// program specifics utils
int passMode();															// open passive data connection

/////// Principal functions ///////
void maker(int opt, int argcpt, char *argvpt[]);                                    // run all ftp protocol

#endif

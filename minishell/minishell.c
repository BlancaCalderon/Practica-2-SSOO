#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "internas.h"
#include "entrada_minishell.h"
#include "ejecutar.h"

static void manejar_sigchild(int signo) {	//función gestor de la señal
	int estado;
	waitpid(-1,&estado, WNOHANG);	/*como pid=-1 espera la finalización  de otro proceso y WNOHANG indica que no se suspende proceso si proceso hijo especificado por pid no se encuentra disponible, estado 					guarda el estado del proceso(esto evita que proceso hijo quede en estado zombie)*/

}

int main(int argc, char *argv[])
{

   char buf[BUFSIZ];
   const char s[2]=";";		//caracter que determina separacion
   char *token;
   struct sigaction sa;		//Define estructura sigaction

   memset(&sa,0,sizeof(sa));	//sustituye tantos caracteres como tamaño de la estructura sigaction por 0 en la estructura sigaction sa
	
   sa.sa_handler=manejar_sigchild;	//indica que gestor de señal será la función manejar_sigchild;
   sa.sa_flags=SA_NOCLDSTOP | SA_RESTART;	/*establece flags, SA_NOCLDSTOP indica que si el número de señal es SIGCHILD no se genera señal cuando proceso hijo se detiene y SA_RESTART que no se 							reestablece la acción de la señal*/
   
   sigaction(SIGCHLD, &sa, NULL);	/*número de señal es SIGCHLD(por lo que cuando se detenga el proceso hijo no se genera señal), la estructura con la que se gestionará la señal es sa y no hay 						estructura que gestionaba anteriormenete la función*/
   


   while (1)
   {
      
      imprimir_prompt();
      leer_linea_ordenes(buf); //almacena ordenes de la entrada estandar como cadena de caracteres
      
   
      if (!strcmp(buf, "exit")) //compara si orden de la entrada estantandar es "exit"
      { 
         exit(0); //sale del programa
      }
      
      else if (es_ord_interna(buf)) //si orden de entrada no es exit y es orden interna
      {
      
         ejecutar_ord_interna(buf); //ejecuta orden interna
         //printf("Interna\n");
      }
      
      else //si es orden externa
      {
	 token=strtok(buf,s);		//separa primer token
	 while(token!=NULL) {		//pasa por el resto de tokens hasta terminar con la orden
		ejecutar_linea_ordenes(token);		//llama a función con cada orden separada
		token=strtok(NULL,s);			//siguiente token
	}
      }
	   
	   
   }

   return 0;
}




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
//#include "ejecutar.h"


int main(int argc, char *argv[])
{

   char buf[BUFSIZ];


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
         printf("%s: Orden externa.\n", buf);
      }
	   
	   
   }

   return 0;
}




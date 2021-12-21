#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "entrada_minishell.h"

void imprimir_prompt()
{
   printf("minishell> ");                 //mostrar en pantalla la cadena que servirá de prompt: entrada de órdenes en la consola 
   fflush(stdout);                        // vacía el buffer intermedio de salida y se envía el texto a la pantalla 
}

//Función elimina el salto de línea
void eliminar_salto_linea(char *cad)
{
   int i, longitud;

   longitud = strlen(cad);                 //Longitud de la orden leída(cad es una cadena de caracteres con la orden leída.)

   for(i = longitud-1; i >= 0; i--)       //Bucle para buscar el carácter de final de línea introducido por fgets
      if (cad[i] == '\n')                 //si caracter introducido es salto de línea
      {
          cad[i] = 0;                     //sustituye por '\0' para indicar el final de orden
          break;
      }
}
     
//Función para leer las órdenes introducidas
void leer_linea_ordenes(char *buf)
{
    
   memset(buf, 0, sizeof(BUFSIZ));              //establece los primeros n(n=tamaño del bufsiz) caracteres del buffer por 0

   if (fgets(buf, BUFSIZ-1, stdin) == NULL)     //fgets almacena la orden leída introduciendo también el carácter de fin de línea 
   { 
      buf[0] = '\0';                            //Caracter indica final de orden
      return;
   }
      
   eliminar_salto_linea(buf);                   //Llama a función eliminar_salto_linea mandándole el buffer(órdenes introducidas)
}


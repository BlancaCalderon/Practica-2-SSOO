#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "entrada_minishell.h"

/* Mostrar en pantalla la cadena que servirá de prompt: entrada de órdenes */
/* en la consola. Además, se vacía el buffer de entrada de órdenes.			*/

void imprimir_prompt()
{
   printf("minishell> "); 
   fflush(stdout);
}

void eliminar_salto_linea(char *cad)
{
   int i, longitud;
	/* Longitud de cad, que es una cadena de caracteres con la orden leída.*/
   longitud = strlen(cad);  

	/* Se busca el carácter de final de línea introducido por fgets y se   */
	/* sustituye por '\0' para indicar el final de orden 						  */
   for(i = longitud-1; i >= 0; i--)
      if (cad[i] == '\n')         
      {
          cad[i] = 0;
          break;
      }
}

/* Función para leer de teclado las órdenes introducidas */    
void leer_linea_ordenes(char *buf)
{
   /* Establece el buffer a 0 */ 
   memset(buf, 0, sizeof(BUFSIZ));
	
	/* fgets almacena la orden leída introduciendo también el carácter 	 */
	/* de fin de línea, que elmininaremos después con la función anterior */
   if (fgets(buf, BUFSIZ-1, stdin) == NULL) 
   { 
      buf[0] = '\0';
      return;
   }

   eliminar_salto_linea(buf);  
}


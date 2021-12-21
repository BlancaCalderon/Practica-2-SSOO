#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

#include "redirecciones.h"

/* Función que abre el archivo situado en la posición indice_entrada + 1 */                                  
void redirec_entrada(char **args, int indice_entrada, int *entrada)
{
	int resOpen;
	
	/* Abre el archivo situado en la posición indice_entrada + 1 de args */
	/* en modo de solo lectura (como indica O_RDONLY)							*/
   if ((resOpen = open(args[indice_entrada + 1], O_RDONLY)) == -1)
   {
   	perror("Error al abrir el archivo");
   	exit(-1);
   } 
   
   else
   {
   	*entrada = resOpen;
   }

}

/* Función que abre el archivo situado en la posición indice_salida + 1 */  
void redirec_salida(char **args, int indice_salida, int *salida)
{
   int resOpen;
   
   /* Abre el archivo situado en la posición indice_salida + 1 de args */
	/* en modo de solo escritura, según indica O_WRONLY, lo crea si no  */
	/* existía anteriormente (O_CREAT) y O_TRUNC indica que si existe,  */
	/* es regular y se puede escribir en él, este se trunca a longitud  */
	/* 0. Permisos establecidos en -rw-rw-rw--								  */
   if ((resOpen = open(args[indice_salida + 1], O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)
   {
   	perror("Error al abrir el archivo");
   	exit(-1);
   }
   
   else
   {
   	*salida = resOpen;
   }

}


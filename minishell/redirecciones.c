#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>


/* funcion que abre el archivo situado en la posicion indice_entrada+1 */
/* de la orden args y elimina de ella la redireccion completa          */
                                  
void redirec_entrada(char **args, int indice_entrada, int *entrada)
{
	int resOpen;
   if ((resOpen = open(args[indice_entrada+1], O_RDONLY)) == -1)
   {
   	perror("Error al abrir el archivo");
   	exit(-1);
   } 
   
   else
   {
   	*entrada = resOpen;
   }

}

/* funcion que abre el archivo situado en la posicion indice_salida+1 */
/* de la orden args y elimina de ella la redireccion completa         */

void redirec_salida(char **args, int indice_salida, int *salida)
{
   int resOpen;
   if ((resOpen = open(args[indice_salida+1], O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)
   {
   	perror("Error al abrir el archivo");
   	exit(-1);
   }
   
   else
   {
   	*salida = resOpen;
   }

}


#include <stdlib.h>

#include "libmemoria.h"

/* Función para liberar espacio de memoria args */
void free_argumentos(char **args)
{
   int i = 0;

	/* Liberamos uno a uno los contenidos de args */
	/* y finalmente, eliminamos args					 */
   while(args[i])
      free(args[i++]);
   free(args);
}

/* Función para liberar de memoria las órdenes separadas por tuberías */
void free_ordenes_pipes(char **ordenes, int **fds, int nordenes)
{
   int i = 0;

   for (i = 0; i < nordenes; i++)
   {
   	/* Eliminamos las órdenes una a una y las tuberías que las separan */
      free(ordenes[i]);
      if (i < (nordenes - 1))
         free(fds[i]);
   }
	
	/* Liberamos de memoria el las órdenes y los descriptores de archivo */
   free(ordenes);
   free(fds);
}

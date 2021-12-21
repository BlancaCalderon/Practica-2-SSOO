#include <stdlib.h>
#include "libmemoria.h"

//Función para liberar espacio de memoria de args
void free_argumentos(char **args)
{
   int i = 0;

   while(args[i])          //Mientras existe argumento
      free(args[i++]);     //Libera zona de memoria que ocupa siguiente argumento
   free(args);             //Libera espacio de memoria qeu ocupa el argumento
}

//Función para liberar la zona de memoria de las ordenes introducidas usando tubería
void free_ordenes_pipes(char **ordenes, int **fds, int nordenes)
{
   int i = 0;

   for (i = 0; i < nordenes; i++)   //Mientras i sea menor que el número de órdenes introducidas
   {
      free(ordenes[i]);             //Libera espacio de memoria de la orden i
      if (i < (nordenes - 1))       //Si es menor que el número de órdenes introducidas menos 1
         free(fds[i]);              //Libera zona de memoria de fds i
   }

   free(ordenes);                   //Libera espacio de memoria de las ordenes
   free(fds);                       //Libera zona de memoria de fds
}

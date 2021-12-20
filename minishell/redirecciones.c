#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>


// funcion que abre el archivo situado en la posicion indice_entrada+1 de la orden args y elimina de ella la redireccion completa          
                                  
void redirec_entrada(char **args, int indice_entrada, int *entrada)
{

	int resOpen;
   	
	if ((resOpen = open(args[indice_entrada+1], O_RDONLY)) == -1)	//abre archivo que acompaña a la redirección de entrada para sólo lectura
   	{

	   	perror("Error al abrir el archivo");			//si ocurre error
	   	exit(-1);						//termina ejecución con status -1
	} 
   
	else								//si no se produce error al abrir archivo
  	{
   		*entrada = resOpen;					//guarda descriptor del archivo devuelto por la función open
  	}

}

// funcion que abre el archivo situado en la posicion indice_salida+1  de la orden args y elimina de ella la redireccion completa         

void redirec_salida(char **args, int indice_salida, int *salida)
{
   	int resOpen;
   	
	if ((resOpen = open(args[indice_salida+1], O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)	/*abre archivo que acompaña a redirección de salida para sólo escritura, además si archivo no existe se crea 													y si existe,es regular y se puede escribir en el se trunca a longitud 0*/
   	{
   		perror("Error al abrir el archivo");	
   		exit(-1);									//si se produce error al abrir archivo se finaliza ejecución
   	}
   
   	else											//si no se produce error al abrir archivo
  	{
   		*salida = resOpen;								//se guarda en variable salida el descriptor de archivo devuelto por la función open
   	}
}

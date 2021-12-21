#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
        
//Funcion que abre el archivo situado en la posicion indice_entrada+1 de la orden args y elimina de ella la redireccion completa                                    
void redirec_entrada(char **args, int indice_entrada, int *entrada)
{

	int resOpen;
   	
	if ((resOpen = open(args[indice_entrada+1], O_RDONLY)) == -1)	/*Abre archivo que acompaña a la redirección de entrada, 
									*opción O_RDONLY indica que arcivo es para sólo lectura*/
   	{

	   	perror("Error al abrir el archivo");			//S ocurre error
	   	exit(-1);						//Trmina ejecución con status -1
	} 
   
	else								//S no se produce error al abrir archivo
  	{
   		*entrada = resOpen;					//Garda descriptor del archivo devuelto por la función open
  	}

}
      
// Funcion que abre el archivo situado en la posicion indice_salida+1  de la orden args y elimina de ella la redireccion completa   
void redirec_salida(char **args, int indice_salida, int *salida)
{
   	int resOpen;			//Variable dónde se guardará descriptor del árchivo devuelto por la función open
	
   	/*Open abre archivo que acompaña a la redirección de salida, LA OPCIÓN O_WRONLY indica que el archivo se abre para sólo escritura,
	 *O_CREAT crea el archivo si no existia anteriormente y 
	 *O_TRUNC indica que si archivo existe, es regular y se puede escribir en él éste se trunca al longitud 0*/
	
	if ((resOpen = open(args[indice_salida+1], O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)		//Si se produce error al abrir el archivo									
   	{
   		perror("Error al abrir el archivo");	
   		exit(-1);						//Si se produce error al abrir archivo se finaliza ejecución
   	}
   
   	else								//Si no se produce error al abrir archivo
  	{
   		*salida = resOpen;					//Se guarda en variable salida el descriptor de archivo devuelto por la función open
   	}
}

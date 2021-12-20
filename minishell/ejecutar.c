#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h> //Eliminar en cuanto quite los printf
#include <errno.h>
#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"


pid_t ejecutar_orden(const char *orden, int *pbackgr)
{
   char **args;
   pid_t pid;
   int indice_ent = -1, indice_sal = -1; // por defecto, no hay ni redirecciones de salida ni de entrada
   int entrada = 0, salida = 1;
  
   if ((args = parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)	
   {
		return(-1);
   }
   else
   {
   	if (indice_ent != -1)   				//si hay redirección de entrada
   	{
   		redirec_entrada(args, indice_ent, &entrada);	//Abre el archivo que acompaña a la redirección de entrada la cual se encuentra en args y guarda en entradas el descriptor de archivo
   		free(args[indice_ent]);				//libera zona de memoria de dónde se encuentra redirección
   		free(args[indice_ent+1]);			//libera zona de memoria del archivo que acompaña a redirección de entrada
   		args[indice_ent] = '\0';
   		args[indice_ent+1] = '\0';
   	}
   	
   	if (indice_sal != -1)					//si hay redirección de salida
   	{
   		redirec_salida(args, indice_sal, &salida);	//abre archivo que acompaña a la redirección de salida(localizada en args) y guarda descriptor del archivo en salida
   		free(args[indice_sal]);				//libera zona de memoria de redirección de salida
   		free(args[indice_sal+1]);			//libera zona de memoria de archivo que acompaña a la redirección de salida
   		args[indice_sal] = '\0';
   		args[indice_sal+1] = '\0';
   	}
   	
   	pid = fork();							//crea proceso hijo
	
		if (pid == -1)  					//si función fork() devuelve -1(error)
		{
			perror("Error en la creación del fork");	//Indicamos al ussuario el error
			exit(-1);					//termina ejecución con status -1(error)
		}
		
		else if (pid == 0)					//si proceso hijo
		{
		
			if (entrada != 0)				//si descriptor del archivo que va con la redirección de entrada no es 0(existe archivo)
			{
				dup2(entrada, STDIN_FILENO);		//duplica descriptor de archivo del archivo de entrada
				if (close(entrada) == -1)		//Cerramos descriptor del archivo de entrada
				{
					perror("Error al cerrar descriptor de archivo de entrada");
					exit(-1);			//termina ejecución con error
				}
			}
			
			if (salida != 1)				//si descriptor del archivo que va con la redirección de entrada no es 0(existe archivo)
			{
				dup2(salida, STDOUT_FILENO);		//duplica descriptor del archivo de salida
				if (close(salida) == -1) 		//Cerramos descriptor del archivo de salida
				{
					perror("Error al cerrar descriptor de archivo de salida");
					exit(-1);			//termina ejecución por error al cerrar descriptor
				}
			}
			
			/*
			 * Sustituimos datos y código del proceso hijo por los de la función introducida en la minishell
			 * Comparamos con -1 para la gestión de posibles errores y el paso de órdenes no contempladas 
			 */
			 
			if (execvp(args[0], args) == -1)		//cambia programa que se está ejecutando y si se produce un error
			{
				perror("");
				exit(-1);				//termina ejecución por error
			}
		}
		
		else 							//si es proceso padre
		{
			if (entrada != 0)				//si descriptor del archivo que va con la redirección de entrada no es 0(existe archivo)
			{
				if (close(entrada) == -1) 		//Cerramos descriptor de entrada
				{
					perror("Error al cerrar descriptor de archivo de entrada");
					exit(-1);
				}
			}
			
			if (salida != 1) 				//si descriptor del archivo que va con la redirección de salida no es 0(existe archivo)
			{
				if (close(salida) == -1) 		//Cerramos descriptor del archivo de salida
				{
					perror("Error al cerrar descriptor de archivo de salida");
					exit(-1);
				}
			}
			
			free_argumentos(args);				//Liberamos la estructura de argumentos creada dinámicamente donde se almacenan las órdenes
		}
		
		/* 
		 * Podemos colocar el return fuera del proceso padre ya que en ningún momento llegará a esta 
		 * sentencia el proceso hijo, o bien se sustituirá su código por el de la orden introducida
		 * o, en caso de error, saldrá con un exit(-1)
		 */ 
		return pid;
   }

   /* Si la linea de ordenes posee tuberias o redirecciones, podra incluir */
   /* aqui, en otras fases de la practica, el codigo para su tratamiento.  */
	

	
}
 
void ejecutar_linea_ordenes(const char *orden)
{
   pid_t pid;
   int backgr;
 
   pid = ejecutar_orden(orden, &backgr); 	//En backgr se devolverá 1 si la orden termina en " &" para indicar que se ejecuta en segundo plano	
	
	if ((!backgr) && pid>0)			//Si la función no corre en segundo plano y es el proceso padre, esperaremos a que acabe el proceso hijo 
	{
		waitpid(pid, NULL, 0);		//Esperamos por la finalización del proceso hijo indicado por pid, sin almacenar el estado
	}
}   

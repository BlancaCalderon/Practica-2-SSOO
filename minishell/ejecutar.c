#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h> //Eliminar en cuanto quite los printf

#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"


pid_t ejecutar_orden(const char *orden, int *pbackgr)
{
   char **args;
   pid_t pid;
   int indice_ent = -1, indice_sal = -1; /* por defecto, no hay < ni > */
   int entrada = 0, salida = 1;
  
   if ((args = parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)
   {
		return(-1);
   }
   else
   {
   	if (indice_ent != -1) 
   	{
   		redirec_entrada(args, indice_ent, &entrada);
   		free(args[indice_ent]);
   		free(args[indice_ent+1]);
   		args[indice_ent] = '\0';
   		args[indice_ent+1] = '\0';
   	}
   	
   	if (indice_sal != -1)
   	{
   		redirec_salida(args, indice_sal, &salida);
   		free(args[indice_sal]);
   		free(args[indice_sal+1]);
   		args[indice_sal] = '\0';
   		args[indice_sal+1] = '\0';
   	}
   	
   	pid = fork();
	
		if (pid == -1) 
		{
			perror("Error en la creación del fork");
			exit(-1);
		}
		
		else if (pid == 0)
		{
		
			if (entrada != 0)
			{
				dup2(entrada, STDIN_FILENO);
				if (close(entrada) == -1) //Cerramos entrada
				{
					perror("Error al cerrar descriptor de archivo de entrada");
					exit(-1);
				}
			}
			
			if (salida != 1)
			{
				dup2(salida, STDOUT_FILENO);
				if (close(salida) == -1) //Cerramos salida
				{
					perror("Error al cerrar descriptor de archivo de salida");
					exit(-1);
				}
			}
			
			/*
			 * Sustituimos datos y código del proceso hijo por los de la función introducida en la minishell
			 * Comparamos con -1 para la gestión de posibles errores y el paso de órdenes no contempladas 
			 */
			 
			if (execvp(args[0], args) == -1)	
			{
				perror("");
				exit(-1);
			}
		}
		
		else 
		{
			if (entrada != 0)
			{
				if (close(entrada) == -1) //Cerramos entrada
				{
					perror("Error al cerrar descriptor de archivo de entrada");
					exit(-1);
				}
			}
			
			if (salida != 1)
			{
				if (close(salida) == -1) //Cerramos salida
				{
					perror("Error al cerrar descriptor de archivo de salida");
					exit(-1);
				}
			}
			
			//Liberamos la estructura de argumentos creada dinámicamente donde se almacenan las órdenes
			free_argumentos(args);	
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


   /* Si la orden es compuesta, podra incluir aqui, en otra fase de la */
   /* practica, el codigo para su tratamiento                          */
 

	//En backgr se devolverá 1 si la orden termina en " &" para indicar que se ejecuta en segundo plano
   pid = ejecutar_orden(orden, &backgr);	
	
	if ((!backgr) && pid>0)		//Si la función no corre en segundo plano y es el proceso padre, esperaremos a que acabe el proceso hijo 
	{
		waitpid(pid, NULL, 0);	//Esperamos por la finalización del proceso hijo indicado por pid, sin almacenar el estado
	}
}   

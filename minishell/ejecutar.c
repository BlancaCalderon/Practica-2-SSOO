#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"

/* Función para crear las tuberías de la orden, se crearán			*/
/* tantas tuberías como órdenes haya separadas por una tubería		*/
/* menos 1. Se almacenan en una lista dinámica							*/
 
int **crear_pipes(int nordenes)
{
	int **pipes = NULL;
	int i;
	
	pipes = (int **) malloc(sizeof(int *) * (nordenes - 1));
	for (i = 0; i < (nordenes - 1); i++)
	{
		pipes[i] = (int *) malloc(sizeof(int) * 2);
		pipe(pipes[i]);
	}
	return pipes;
}

pid_t ejecutar_orden(const char *orden, int entrada, int salida, int *pbackgr)
{
   char **args;
   pid_t pid;
   int indice_ent = -1, indice_sal = -1; /* por defecto, no hay < ni > */
  
   if ((args = parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)
   {
   	/* Si la entrada o salida no es la estándar, cerramos por seguridad	*/
   	if (entrada != 0)
		{
			if (close(entrada) == -1)
			{
				perror("Error al cerrar descriptor de archivo de entrada");
				exit(-1);
			}
		}
			
		if (salida != 1)
		{
			if (close(salida) == -1)
			{
				perror("Error al cerrar descriptor de archivo de salida");
				exit(-1);
			}
		}
		return(-1);
   }
   else
   {
   
   	/* Si existe redirección de entrada, abrimos el archivo indicado y	  */
   	/* liberamos la posición de memoria del signo < o > y del nombre 		  */
   	/* del archivo, que apuntaremos a '\0' para evitar problemas			  */
   	
   	if (indice_ent != -1) 
   	{
   		redirec_entrada(args, indice_ent, &entrada);
   		free(args[indice_ent]);
   		free(args[indice_ent + 1]);
   		args[indice_ent] = '\0';
   		args[indice_ent + 1] = '\0';
   	}
   	
   	/* Mismo procedimiento en el caso de que exista redirección de salida  */
   	if (indice_sal != -1)
   	{
   		redirec_salida(args, indice_sal, &salida);
   		free(args[indice_sal]);
   		free(args[indice_sal + 1]);
   		args[indice_sal] = '\0';
   		args[indice_sal + 1] = '\0';
   	}
   	
   	pid = fork();			/* Creamos el proceso hijo */
	
		if (pid == -1) 
		{
			perror("Error en la creación del fork");
			exit(-1);
		}
		
		else if (pid == 0)	/* Si proceso hijo */
		{
			/* Si existe archivo de entrada de redirección, cerramos el   */
			/*	descriptor de archivo estándar y copiamos el descriptor de */
			/*	archivo de la entrada en la posición que deja libre		  */
			
			if (entrada != 0)
			{
				dup2(entrada, STDIN_FILENO);
				if (close(entrada) == -1)
				{
					perror("Error al cerrar descriptor de archivo de entrada");
					exit(-1);
				}
			}
			
			/* Mismo proceso anterior, pero en este caso con la salida	  */
			
			if (salida != 1)
			{
				dup2(salida, STDOUT_FILENO);
				if (close(salida) == -1) //Cerramos salida
				{
					perror("Error al cerrar descriptor de archivo de salida");
					exit(-1);
				}
			}
			
			/* Sustituimos datos y código del proceso hijo por los de la 	  */
			/* función introducida en la minishell. Comparamos con -1 para   */
			/* gestión de posibles errores y paso de órdenes no contempladas */
			 
			if (execvp(args[0], args) == -1)	
			{
				perror("Execvp");
				exit(-1);
			}
		}
		
		else 		/* Si es proceso padre */
		{
			/* Si existe archivo de redirección de entrada, lo cerramos */
			if (entrada != 0)
			{
				if (close(entrada) == -1)
				{
					perror("Error al cerrar descriptor de archivo de entrada");
					exit(-1);
				}
			}
			
			/* Si existe archivo de redirección de salida, lo cerramos */
			if (salida != 1)
			{
				if (close(salida) == -1)
				{
					perror("Error al cerrar descriptor de archivo de salida");
					exit(-1);
				}
			}
			
			/* Liberamos la estructura de argumentos creada */
			/* dinámicamente donde se almacenan las órdenes */
			
			free_argumentos(args);	
		}
		
		return pid;
   }	
}
 
void ejecutar_linea_ordenes(const char *orden)
{
	char **ordenes;
	int nordenes;
	int entrada = STDIN_FILENO, salida = STDOUT_FILENO;
   pid_t *pids = NULL;
   int **pipes;
   int backgr, i;
 	
 	/* Extraemos las órdenes separadas por tuberías */
	ordenes = parser_pipes(orden, &nordenes);
	
	/* Creamos array de forma dinámica donde almacenar */
	/* los pids de las diferentes órdenes					*/
	
	pids = (pid_t *) malloc(sizeof(pid_t) * nordenes);
	
	/* Creamos todas las pipes que aparezcan en la orden */
	pipes = crear_pipes(nordenes);
	
   for (i = 0; i < nordenes; i++) 
   {
   	/* Si es la orden de la izquierda, utilizará entrada estándar   */
   	/* y como salida la primera tubería										 */
   	if (i == 0) 
   	{
   		if (nordenes > 1) 
   		{
   			salida = pipes[0][1];
   		}
   	}
   	
   	/* Si es la orden de la derecha y existe más de una orden 		 */
   	/* su salida será la estándar y su entrada la última tubería	 */
   	else if (i == (nordenes -1) && (nordenes > 1))
   	{
   		entrada = pipes[nordenes - 2][0];
   		salida = 1;
   	}
   	
   	/* Si es una orden intermedia, su entrada será la tubería 		 */
   	/* anterior y su salida será la tubería siguiente					 */
   	else
   	{
   		entrada = pipes[i - 1][0];
   		salida = pipes[i][1];
   	}
   	
   	/* En backgr se devolverá 1 si la orden termina en " &" */
		/* para indicar que se ejecuta en segundo plano			  */
   	
   	pids[i] = ejecutar_orden(ordenes[i], entrada, salida, &backgr);
   	
   	/* Si la función no corre en segundo plano y es el proceso padre, */
   	/* esperaremos a que finalice el proceso hijo						   */
   	
		if ((!backgr) && pids[nordenes - 1]>0)		
		{
			waitpid(pids[nordenes - 1], NULL, 0);	
		}
	}
	
	/* Liberamos las estructuras creadas dinámicamente de memoria */
	free_ordenes_pipes(ordenes, pipes, nordenes);
	free(pids);
}   

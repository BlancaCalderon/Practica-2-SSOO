#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "internas.h"
#include "entrada_minishell.h"
#include "ejecutar.h"

static void manejar_sigchild(int signo)	//Función gestor de la señal
{
	int estado;
	
	/*
	 *	pid = -1 indica que waitpid va a esperar por la finalización de cualquier proceso hijo.  
	 *	WNOHANG indica que el waitpid debería terminar inmediatamente si no hay ningún proceso 
	 * hijo que haya finalizado en vez de suponer una espera bloqueante.
	 * estado va a guardar el estado de terminación del proceso para verificar que el proceso
	 * realmente ha terminado.
	 */
	waitpid(-1, &estado, WNOHANG); 			
}

int main(int argc, char *argv[])
{

   char buf[BUFSIZ];
   /* Creamos una estructura sigaction que contendrá la información 
    * que asignaremos a la función sigaction.
    * Los campos que vamos a utilizar de dicha estructura son:
    * sa_handler, que especifica la acción que va a ser asociada con signum
    * En nuestro caso, signum va a ser la señal SIGCHLD
    * sa_flags, donde se establcerán las "opciones" que usaremos en sigaction
    */
   struct sigaction sa;	
   char *token;
   
   //Establecemos la estructura sa creada a todo ceros
   memset(&sa, 0, sizeof(sa));	
   //Indica que el gestor de señal será la función manejar_sigchild		
   sa.sa_handler = manejar_sigchild;	 
   //Establece flags, SA_NOCLDSTOP indica que si el número de señal es SIGCHILD no se genera señal cuando 
   //el proceso hijo se detiene y SA_RESTART que no se reestablece la acción de la señal
   sa.sa_flags = SA_NOCLDSTOP | SA_RESTART;
   
   sigaction(SIGCHLD, &sa, NULL);

   while (1)
   {
   
	   imprimir_prompt();				//Imprimimos el mensaje del prompt
	   
	   leer_linea_ordenes(buf);		//Leemos la linea introducida por teclado
	   
	   if (strcmp(buf, ""))				//Controlamos que lo introducido no sea un Enter, ya que strtok da error
	   {
	   
	   	/* ---  FUNCIONALIDAD AÑADIDA  ---
	   	 * Con strtok separamos las diferentes sentencias separadas por ;
			 * Gracias a separar las sentencias antes de hacer el tratamiento de las órdenes
			 * podemos concatenar tanto internas como externas, e incluso la finalización de la 
			 * minishell si así se deseara, aunque para esto no podrá introducirse un espacio antes
			 * de la orden exit. Esta decisión se toma para evitar que el usuario pueda salir de la minishell
			 * introduciendo simplemente " exit"
			 */
			  
			token = strtok(buf, ";");	
			do 
			{
				//Comparamos con exit para salir de la minishell en caso de que se haya introducido dicha orden
				if (!strcmp(token, "exit"))		
				{
					exit(0);
				}
				else
		  		{
					if (es_ord_interna(token))	//Si es orden interna, se ejecutará la orden interna
					{
						ejecutar_ord_interna(token);
					}	
					else								//Ejecuta la orden externa en caso contrario
					{
						ejecutar_linea_ordenes(token);
					}	
				}
				token=strtok(NULL, ";");		//Cogemos el siguiente token de los almacenados en token
			} while(token!=NULL);				//Realizamos el interior del bloque do mientras siga habiendo sentencias por ejecutar 
		}
   }

   return 0;
}




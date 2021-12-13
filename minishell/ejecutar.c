#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"

pid_t ejecutar_orden(const char *orden, int *pbackgr)
{
 	char **args;
	pid_t pid;
	int indice_ent = -1, indice_sal = -1; /* por defecto, no hay < ni > */
	  
	if ((args = parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)	//se pulsa enter
	{
		return(-1);
	 	
	}


	pid=fork();	//crea proceso hijo
	if(pid==-1)	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(pid>0) {	//proceso padre
		free_argumentos(args);		//libera memoria dinámica
		return(pid);			//devuelte pid del proceso hijo	
		
	}
	else {		//proceso hijo
		execvp(args[0], args);
		return(pid);
	}
	exit(EXIT_SUCCESS);
	
 	  

   /* Si la linea de ordenes posee tuberias o redirecciones, podra incluir */
   /* aqui, en otras fases de la practica, el codigo para su tratamiento.  */
		
}
 
void ejecutar_linea_ordenes(const char *orden)
{
	pid_t pid;		
	int backgr;

   /* Si la orden es compuesta, podra incluir aqui, en otra fase de la */
   /* practica, el codigo para su tratamiento                          */
 
	
	if(orden[(sizeof(orden)/sizeof(*orden))-1 =='&']) {	//si existe &
		backgr=0;
	}
		
	else {				
		backgr=1;
	}
	
	pid = ejecutar_orden(orden, &backgr);		//proceso hijo es ejecutar_orden
  	if((pid>0)&&(backgr==0)) {	//proceso padre y no existe &
		pid_t wait();		//espera a que termine proceso hijo	
	}
	
}


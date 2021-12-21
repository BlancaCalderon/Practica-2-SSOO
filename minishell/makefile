# Definición de variable CC para representar el compilador usado
CC=gcc

# Definición de variable CFLAGS para representar las opciones de compilación 
CFLAGS=-g -Wall

# Regla para la obtención del ejecutable de la minishell a partir de sus dependencias.
minishell: minishell.o ejecutar.o entrada_minishell.o libmemoria.o redirecciones.o libshell.a 
	$(CC) $(CFLAGS)  minishell.o libshell.a entrada_minishell.o ejecutar.o libmemoria.o redirecciones.o -L./-lshell -o minishell 
	

# Regla para la obtención de la biblioteca estática libshell.a a partir de sus dependencias.
libshell.a: parser.o internas.o 
	ar -rs libshell.a parser.o internas.o 

# Regla para la obtención de minishell.o a partir de sus dependencias.
minishell.o: minishell.c ejecutar.h internas.h entrada_minishell.h  
	$(CC) $(CFLAGS) -c minishell.c 

# Regla para la obtención de ejecutar.o a partir de sus dependencias.
ejecutar.o: ejecutar.c parser.h libmemoria.h ejecutar.h redirecciones.h
	$(CC) $(CFLAGS) -c ejecutar.c 

# Regla para la obtención de libmemoria.o a partir de sus dependencias.
libmemoria.o: libmemoria.c libmemoria.h 
	$(CC) $(CFLAGS) -c libmemoria.c 

# Regla para la obtención de redirecciones.o a partir de sus dependencias.
redirecciones.o: redirecciones.c redirecciones.h
	$(CC) $(CFLAGS) -c redirecciones.c 

# Regla para la obtención de entrada_minishell.o a partir de sus dependencias.
entrada_minishell.o: entrada_minishell.c entrada_minishell.h 
	$(CC) $(CFLAGS) -c entrada_minishell.c 

# Asegura que make clean funcione aunque exista un archivo llamado "clean" en el directorio
.PHONY: clean 

# Borra los archivos objeto indicados y el ejecutable.
# Mantenemos parser.o e internas.o.
clean:
	rm -f minishell.o ejecutar.o entrada_minishell.o libmemoria.o redirecciones.o minishell 
	

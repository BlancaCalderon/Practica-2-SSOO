CC= gcc #redefinición de variable CC para representar el compilador usado
CFLAGS=-g -Wall #redefinición de variable CC para representar las opciones de compilación

minishell: minishell.o ejecutar.o entrada_minishell.o libmemoria.o redirecciones.o libshell.a #declaración del archivo que se quiere crear y sus dependencias
	$(CC) $(CFLAGS)  minishell.o libshell.a entrada_minishell.o ejecutar.o libmemoria.o redirecciones.o -L./-lshell -o minishell #orden para la compilación del archivo con variables de makefile

libshell.a: parser.o internas.o #declaración del archivo anteriormente solicitado y sus dependencias
	ar -rs -c libshell.a parser.o internas.o #orden para la creación de la biblioteca estática

minishell.o: minishell.c ejecutar.h internas.h entrada_minishell.h  #declaración del archivo anteriormente solicitado y sus dependencias
	$(CC) $(CFLAGS) -c minishell.c #orden para la compilación del archivo con variables de makefile

ejecutar.o: ejecutar.c parser.h libmemoria.h ejecutar.h #declaración del archivo anteriormente solicitado y sus dependencias
	$(CC) $(CFLAGS) -c ejecutar.c #orden para la compilación del archivo con variables de makefile

libmemoria.o: libmemoria.c libmemoria.h #declaración del archivo anteriormente solicitado y sus dependencias
	$(CC) $(CFLAGS) -c libmemoria.c #orden para la compilación del archivo con variables de makefile

redirecciones.o: redirecciones.c #declaración del archivo anteriormente solicitado y sus dependencias
	$(CC) $(CFLAGS) -c redirecciones.c #orden para la compilación del archivo con variables de makefile

entrada_minishell.o: entrada_minishell.c entrada_minishell.h #declaración del archivo anteriormente solicitado y sus dependencias
	$(CC) $(CFLAGS) -c entrada_minishell.c #orden para la compilación del archivo con variables de makefile

.PHONY: clean #asegura que make clean funcione aunque exista un archivo llamado "clean" en el directorio
clean:
	rm -f minishell.o ejecutar.o entrada_minishell.o libmemoria.o redirecciones.o minishell #borra los archivos objeto y el ejecutable

CC= gcc
CFLAGS=-g -Wall

minishell: minishell.o ejecutar.o entrada_minishell.o libmemoria.o redirecciones.o libshell.a
	$(CC) $(CFLAGS)  minishell.o libshell.a entrada_minishell.o ejecutar.o libmemoria.o redirecciones.o -L./-lshell -o minishell

libshell.a: parser.o internas.o
	ar -rs -c libshell.a parser.o internas.o

minishell.o: minishell.c ejecutar.h internas.h entrada_minishell.h 
	$(CC) $(CFLAGS) -c minishell.c

ejecutar.o: ejecutar.c parser.h libmemoria.h ejecutar.h 
	$(CC) $(CFLAGS) -c ejecutar.c

libmemoria.o: libmemoria.c libmemoria.h
	$(CC) $(CFLAGS) -c libmemoria.c

redirecciones.o: redirecciones.c 
	$(CC) $(CFLAGS) -c redirecciones.c

entrada_minishell.o: entrada_minishell.c entrada_minishell.h
	$(CC) $(CFLAGS) -c entrada_minishell.c

.PHONY: clean
clean:
	rm -f minishell.o ejecutar.o entrada_minishell.o libmemoria.o redirecciones.o


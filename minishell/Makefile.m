CC= gcc
CFLAGS=-g -wall

minishell: minishell.o internas.o parser.o
	&(CC) &(CFLAGS) -lshell -o minishell minishell.o internas.o parser.o

minishell.o: minishell.c internas.h entrada_minishell.h ejecutar.h
	&(CC) &(CFLAGS) -c minishell.c

.PHONY clean
clean:
	rm -f minishell.o


# $Id$

GCC=gcc
FILES=tcp_serveur.c tcp_client.c server.c serveurMap.c monitoring.c
OBJ=$(FILES:%.c=%.o)
BIN=tcp_serveur tcp_client server serveurMap monitoring

all: $(OBJ)
	$(GCC) -o tcp_serveur tcp_serveur.o -lpthread
	$(GCC) -o server server.o -lpthread
	$(GCC) -o tcp_client tcp_client.o
	$(GCC) -o serveurMap serveurMap.o
	$(GCC) -o monitoring monitoring.o

clean:
	rm -f *~ *.o $(BIN)

.SUFFIXES: .c .o

.zf.res:
	$(GCC) -c $<

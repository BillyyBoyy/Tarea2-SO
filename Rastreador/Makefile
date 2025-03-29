CC = gcc
CFLAGS = -Wall -g
OBJS = rastreador.o syscall_map.o

all: rastreador

rastreador: $(OBJS)
	$(CC) $(CFLAGS) -o rastreador $(OBJS)

rastreador.o: rastreador.c syscall_map.h

syscall_map.o: syscall_map.c syscall_map.h

clean:
	rm -f *.o rastreador

CC=gcc
CFLAGS=-c -Wall

all: cortex

cortex: cortex.o lib/utils.o
	$(CC) cortex.o lib/utils.o -o cortex

cortex.o: cortex.c
	$(CC) $(CFLAGS) cortex.c

utils.o: lib/utils.c
	$(CC) $(CFLAGS) lib/utils.c

clean:
	rm cortex *.o lib/*.o

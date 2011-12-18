CC=gcc
CFLAGS=-c -Wall

all: cortex

cortex: cortex.o lib/utils.o lib/damn.o
	$(CC) cortex.o lib/utils.o lib/damn.o -o cortex

cortex.o: cortex.c
	$(CC) $(CFLAGS) cortex.c

utils.o: lib/utils.c
	$(CC) $(CFLAGS) lib/utils.c

damn.o: lib/damn.c
	$(CC) $(CFLAGS) lib/damn.c

clean:
	rm cortex *.o lib/*.o

CC=gcc
CFLAGS=-c -Wall

all: cortex

cortex: cortex.o lib/utils.o lib/damn.o lib/strmap.o
	$(CC) cortex.o lib/utils.o lib/damn.o lib/strmap.o -o cortex

cortex.o: cortex.c
	$(CC) $(CFLAGS) cortex.c

utils.o: lib/utils.c
	$(CC) $(CFLAGS) lib/utils.c

damn.o: lib/damn.c
	$(CC) $(CFLAGS) lib/damn.c

strmap.o: lib/strmap.c
	$(CC) $(CFLAGS) lib/strmap.c

clean:
	rm cortex *.o lib/*.o

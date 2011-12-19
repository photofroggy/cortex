CC=gcc
CFLAGS=-c -Wall
CLINKS=-lWs_32

all: cortex

cortex: cortex.o lib/utils.o lib/damn.o lib/strmap.o
	$(CC) cortex.o lib/utils.o lib/damn.o lib/strmap.o -o cortex

cortex.o: cortex.c
	$(CC) $(CFLAGS) cortex.c $(CLINKS)

utils.o: lib/utils.c
	$(CC) $(CFLAGS) lib/utils.c $(CLINKS)

damn.o: lib/damn.c
	$(CC) $(CFLAGS) lib/damn.c $(CLINKS)

strmap.o: lib/strmap.c
	$(CC) $(CFLAGS) lib/strmap.c $(CLINKS)

clean:
	rm cortex *.o lib/*.o

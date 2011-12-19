CC=gcc
CFLAGS=-c -Wall

all: ptest

ptest: ptest.o lib/damn.o lib/strmap.o
	$(CC) ptest.o lib/damn.o lib/strmap.o -o ptest

ptest.o: ptest.c
	$(CC) $(CFLAGS) ptest.c

damn.o: lib/damn.c
	$(CC) $(CFLAGS) lib/damn.c

strmap.o: lib/strmap.c
	$(CC) $(CFLAGS) lib/strmap.c

clean:
	rm ptest *.o lib/*.o

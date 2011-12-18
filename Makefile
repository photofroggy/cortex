CC=gcc
CFLAGS=-c -Wall

all: cortex

cortex: main.o utils.o
	$(CC) main.o utils.o -o cortex

main.o: main.c
	$(CC) $(CFLAGS) main.c

utils.o: utils.c
	$(CC) $(CFLAGS) utils.c

clean:
	rm cortex *.o

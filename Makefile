all: cortex

cortex: main.o utils.o
	gcc main.o utils.o -o cortex

main.o: main.c
	gcc -c main.c

utils.o: utils.c
	gcc -c utils.c

clean:
	rm cortex *.o

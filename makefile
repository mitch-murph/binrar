# makefile for generating binrar.out

CC = gcc
CFLAGS = -Wall -Werror -ansi -lm

binrar.out: main.o vector.o tree.o hashmap.o
	$(CC) $(CFLAGS) -o binrar.out main.o vector.o tree.o hashmap.o

main.o: main.c
	$(CC) $(CFLAGS) -c  -o main.o main.c

vector.o: vector.c vector.h 
	$(CC) $(CFLAGS) -c  -o vector.o vector.c

tree.o: tree.c tree.h 
	$(CC) $(CFLAGS) -c  -o tree.o tree.c

hashmap.o: hashmap.c hashmap.h 
	$(CC) $(CFLAGS) -c  -o hashmap.o hashmap.c

clean:
	rm hashmap.o tree.o vector.o main.o binrar.out

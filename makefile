# makefile for generating binrar.out

CC = gcc
CFLAGS = -Wall -Werror -ansi -lm

binrar.out: main.o vector.o
	$(CC) $(CFLAGS) -o binrar.out main.o vector.o

main.o: main.c
	$(CC) $(CFLAGS) -c  -o main.o main.c

vector.o: vector.c vector.h 
	$(CC) $(CFLAGS) -c  -o vector.o vector.c

clean:
	rm vector.o main.o binrar.out

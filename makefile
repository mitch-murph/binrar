# makefile for generating binrar.out

CC = gcc
CFLAGS = -Wall -Werror -ansi -lm
DEPS = vector.h tree.h hashmap.h
OBJ = main.o vector.o tree.o hashmap.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

binrar.out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) binrar.out

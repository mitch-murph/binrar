# makefile for generating binrar.out

CC = gcc
CFLAGS = -ansi -lm
DEPS = vector.h tree.h hashmap.h compress.h bit_array.h filepackager.h
OBJ = main.o vector.o tree.o hashmap.o compress.o bit_array.o filepackager.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

binrar.out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) binrar.out

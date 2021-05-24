# makefile for generating binrar.out

CC = gcc
CFLAGS = -Wall -Werror -ansi -lm
SRCDIR = src
OBJDIR = obj
EXE = binrar.out
SRC = $(wildcard $(SRCDIR)/*.c)
DEPS = $(wildcard $(SRCDIR)/*.h)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(EXE)

$(EXE): $(OBJ) $(OBJDIR)/main.o
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJDIR)/main.o: main.c
	$(CC) -c -o $@ $^ $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(OBJDIR) $(EXE)
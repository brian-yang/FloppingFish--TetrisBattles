CC = gcc
CFLAGS = -Wall -lpanel -lncurses
LDFLAGS = -lpanel -lncurses
BUGFLAGS = -g

SRC = tetris.c main.c
EXEC = tetris

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $^ $(CFLAGS) -o $@

clean:
	rm $(EXEC)
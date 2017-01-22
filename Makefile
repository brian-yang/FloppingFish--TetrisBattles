CFLAGS = -Wall -lpanel -lncurses
NCURSES_FLAGS = -lpanel -lncurses
GDB_FLAG = -g

SRC = tetris.c main.c
EXEC = tetris

all: compile

compile: $(SRC)
	gcc $^ $(CFLAGS) -o $(EXEC)
run:
	./$(EXEC)
clean:
	rm -f $(EXEC)
	rm -f *~ *.out

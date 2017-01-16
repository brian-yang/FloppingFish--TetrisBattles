compile: board.o
	gcc board.o tetris.c -lncurses -lpanel -o tetris
board.o: board.c
	gcc -c board.c
run: compile
	./tetris
clean:
	rm -f *.out *.o tetris

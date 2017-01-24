CFLAGS = -Wall -Werror -lpanel -lncurses
NCURSES_FLAGS = -lpanel -lncurses
GDB_FLAG = -g

SRC = tetris.c main.c
EXEC = tetris

all: server client

server: server.o networking.o
	gcc networking.o server.o -o server

client: client.o networking.o main.o tetris.o
	gcc networking.o tetris.o main.o client.o $(CFLAGS) -o client
#==================================================
main.o: main.c common.h
	gcc -c main.c

tetris.o: tetris.c common.h
	gcc -c tetris.c

server.o: server.c networking.h
	gcc -c server.c

client.o: client.c networking.h common.h
	gcc -c client.c

networking.o: networking.c networking.h
	gcc -c networking.c
#==================================================
run-server:
	./server
run-client:
	./client
clean:
	rm server client
	rm *.o
	rm *~
vs: server
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./server
vc: client
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./client

# all: compile

# compile: $(SRC)
# 	gcc $^ $(CFLAGS) -o $(EXEC)
# run:
# 	./$(EXEC)
# clean:
# 	rm -f $(EXEC)
# 	rm -f *~ *.out
# valtetris: all
# 	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./$(EXEC)

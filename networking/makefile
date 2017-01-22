all: server client

server: server.o networking.o
	gcc -o server server.o networking.o

client: client.o networking.o
	gcc -o client client.o networking.o

server.o: server.c networking.h
	gcc -c server.c

client.o: client.c networking.h
	gcc -c client.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm server client
	rm *.o
	rm *~
vs: server
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./server
vc: client
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include <string.h>
#include <errno.h>

#include "networking.h"

#define NUM_USERS 30

void sub_server1( int connection, int read_pipe, int write_pipe, char* buffer);
void sub_server2( int connection, int read_pipe, int write_pipe, char* buffer);

int main() {
  printf("[server] Server started!\n");

  int sd; // initial file descriptor for connection between server and client
  int pipes[2 * NUM_USERS]; // pipes for communicating between server and subprocesses
  int c; // server-client connection

  // Pipe setup
  int i;
  for (i = 0; i < NUM_USERS; i++) {
    pipe(&pipes[2*i]);
  }

  int pipes_in_use = pipes[0];
  int waiting_room_pipes[2] = {-1, -1};
  int num_users = 0;

  //===============================================================
  //===============================================================

  sd = server_setup();

  while (num_users < NUM_USERS) {

    c = server_connect( sd );

    pipes_in_use += 2;
    num_users += 1;

    int f = fork();
    if ( f == 0 ) {

      close(sd);

      close(pipes_in_use - 1);
      char buffer[MESSAGE_BUFFER_SIZE];
      read(pipes_in_use - 2, buffer, 1024);

      char* pointer_buffer = buffer;

      int pipe_ids[2];
      pipe_ids[0] = atoi(strsep(&pointer_buffer, "-"));
      pipe_ids[1] = atoi(pointer_buffer);

      int converted_num = htonl(-1);
      write(c, &converted_num, sizeof(converted_num));

      char buffer2[MESSAGE_BUFFER_SIZE];
      if (pipe_ids[0] < pipes_in_use - 2) {
	sub_server1(c, pipes_in_use - 2, pipe_ids[1], buffer2);
      } else{
	sub_server2(c, pipes_in_use - 2, pipe_ids[1], buffer2);
      }

      close(pipes_in_use - 2);

      exit(0);
    }
    else {
      close(pipes_in_use - 2);
      if (waiting_room_pipes[0] == -1) {
	waiting_room_pipes[0] = pipes_in_use - 2;
	waiting_room_pipes[1] = pipes_in_use - 1;
      } else {
	char pipe_ids[1024];
	char pipe_ids2[1024];

	sprintf(pipe_ids, "%d-%d\n", pipes_in_use - 2, pipes_in_use - 1);
	write(waiting_room_pipes[1], pipe_ids, strlen(pipe_ids) + 1);
	close(waiting_room_pipes[1]);

	sprintf(pipe_ids2, "%d-%d\n", waiting_room_pipes[0], waiting_room_pipes[1]);
	write(pipes_in_use - 1, pipe_ids2, strlen(pipe_ids2) + 1);

	waiting_room_pipes[0] = -1;
	waiting_room_pipes[1] = -1;
      }
      close(c);
    }
  }
  printf("[server] Server has reached maximum capacity. Please try again later!\n");
  return 0;
}

void sub_server1( int connection, int read_pipe, int write_pipe, char* buffer) {
  int read_num = 0;
  read(connection, &read_num, sizeof(read_num));
  while (1) {
    write( write_pipe, &read_num, sizeof(read_num) );

    int read_num2 = 0;
    read(read_pipe, &read_num2, sizeof(read_num2));

    int converted_num = htonl(read_num2);
    write(connection, &converted_num, sizeof(converted_num));

    int received_int = 0;
    read(connection, &received_int, sizeof(received_int));
  }
  close(connection);
  close(read_pipe);
  close(write_pipe);
}

void sub_server2( int connection, int read_pipe, int write_pipe, char* buffer) {
  int read_num = 0;
  read(connection, &read_num, sizeof(read_num));
  while (1) {
    write( write_pipe, &read_num, sizeof(read_num) );

    int read_num2 = 0;
    read(read_pipe, &read_num2, sizeof(read_num2));

    int converted_num = htonl(read_num2);
    write(connection, &converted_num, sizeof(converted_num));

    int received_int = 0;
    read(connection, &received_int, sizeof(received_int));
  }
  close(connection);
  close(read_pipe);
  close(write_pipe);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include <string.h>
#include <errno.h>

#include "networking.h"

#define NUM_USERS 10

void sub_server1( int connection, int read_pipe, int write_pipe, char* buffer);
void sub_server2( int connection, int read_pipe, int write_pipe, char* buffer);

/* union semun { */
/*   int val; /\* Value for SETVAL *\/ */
/*   struct semid_ds *buf; /\* Buffer for IPC_STAT, IPC_SET *\/ */
/*   unsigned short *array; /\* Array for GETALL, SETALL *\/ */
/*   struct seminfo *__buf; /\* Buffer for IPC_INFO */
/* 			    (Linux specific) *\/ */
/* }; */

/* key_t get_semaphore() { */
/*   key_t sem = ftok("/usr", 30); */
/*   if (errno) { */
/*     printf("Ftok error #%d: %s\n", errno, strerror(errno)); */
/*     exit(-1); */
/*   } */
/*   return sem; */
/* } */

/* key_t get_sharedmem() { */
/*   key_t shm = ftok("/home", 30); */
/*   if (errno) { */
/*     printf("Ftok error #%d: %s\n", errno, strerror(errno)); */
/*     exit(-1); */
/*   } */
/*   return shm; */
/* } */

int main() {
  int sd; // initial file descriptor for connection between server and client
  struct connection_info* c; // used to store connection info from clients
  int pipes[2 * NUM_USERS]; // pipes for communicating between server and subprocesses

  int fd;

  // Pipe setup
  int i;
  for (i = 0; i < NUM_USERS; i++) {
    pipe(&pipes[2*i]);
  }

  int pipes_in_use = pipes[0];
  int waiting_room_pipes[2] = {-1, -1};

  //===============================================================
  //===============================================================

  sd = server_setup();

  while (pipes_in_use < 2 * NUM_USERS) {

    c = server_connect( sd );

    pipes_in_use += 2;

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

      char buffer2[MESSAGE_BUFFER_SIZE];
      if (pipe_ids[0] < pipes_in_use - 2) {
	sub_server1(c->fd, pipes_in_use - 2, pipe_ids[1], buffer2);
      } else{
	sub_server2(c->fd, pipes_in_use - 2, pipe_ids[1], buffer2);
      }

      close(pipes_in_use - 2);

      free(c);
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

      close(c->fd);
      free(c);
    }
  }
  return 0;
}

void sub_server1( int connection, int read_pipe, int write_pipe, char* buffer) {
  read(connection, buffer, sizeof(buffer));
  while (1) {
    write( write_pipe, buffer, strlen(buffer) + 1 );
    read(read_pipe, buffer, sizeof(buffer));
    write(connection, buffer, strlen(buffer) + 1);
    read(connection, buffer, sizeof(buffer));
  }
  close(connection);
  close(read_pipe);
  close(write_pipe);
}

void sub_server2( int connection, int read_pipe, int write_pipe, char* buffer) {
  read(connection, buffer, sizeof(buffer));
  while (1) {
    write( write_pipe, buffer, strlen(buffer) + 1 );
    read(read_pipe, buffer, sizeof(buffer));
    write(connection, buffer, strlen(buffer) + 1);
    read(connection, buffer, sizeof(buffer));
  }
  close(connection);
  close(read_pipe);
  close(write_pipe);
}

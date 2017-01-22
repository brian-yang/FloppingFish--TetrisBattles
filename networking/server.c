#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include <string.h>
#include <errno.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );

union semun {
  int val; /* Value for SETVAL */
  struct semid_ds *buf; /* Buffer for IPC_STAT, IPC_SET */
  unsigned short *array; /* Array for GETALL, SETALL */
  struct seminfo *__buf; /* Buffer for IPC_INFO
			    (Linux specific) */
};

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

  fd = open("waitingroom.log", O_TRUNC | O_CREAT, 0644);
  close(fd);

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
      printf("ok2!\n");
      printf("%d\n", pipes_in_use - 2);
      read(pipes_in_use - 2, buffer, 1024);

      printf("testing! %s\n", buffer);
      close(pipes_in_use - 2);

      fd = open("waitingroom.log", O_WRONLY | O_APPEND);
      write(fd, buffer, strlen(buffer));
      close(fd);

      /* strncpy(buffer, c->ip, strlen(c->ip) + 1); */
      /* write(c->fd, buffer, strlen(buffer) + 1); */

      //sub_server( c->fd );

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
	printf("ok!\n");
	write(waiting_room_pipes[1], pipe_ids, strlen(pipe_ids));
	printf("ok3!\n");
	close(waiting_room_pipes[1]);

	printf("ok5!\n");
	sprintf(pipe_ids2, "%d-%d\n", waiting_room_pipes[0], waiting_room_pipes[1]);
	printf("ok6!\n");
	write(pipes_in_use - 1, pipe_ids2, strlen(pipe_ids2));
	printf("ok7!\n");

	waiting_room_pipes[0] = -1;
	waiting_room_pipes[1] = -1;
      }

      close(c->fd);
      free(c);
    }
  }
  return 0;
}

void sub_server( int sd ) {

  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, strlen(buffer) + 1);
  }

}
void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}

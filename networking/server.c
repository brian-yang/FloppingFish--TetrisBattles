#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );

int main() {

  int sd;
  struct connection_info* c;

  sd = server_setup();
    
  while (1) {

    c = server_connect( sd );
    
    int f = fork();
    if ( f == 0 ) {

      close(sd);

      char buffer[MESSAGE_BUFFER_SIZE];

      strncpy(buffer, c->ip, strlen(c->ip) + 1);
      write(c->fd, buffer, strlen(buffer));

      //sub_server( connection );
      
      free(c);
      exit(0);
    }
    else {
      close(c->fd );
    }
  }
  return 0;
}

void sub_server( int sd ) {

  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));    
  }
  
}
void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

int main( int argc, char *argv[] ) {

  char *host;
  if (argc != 2 ) {
    printf("host not specified, conneting to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];

  int sd;

  sd = client_connect( host );

  char buffer[MESSAGE_BUFFER_SIZE];

  int not_found_opponent = 1;

  printf("Waiting for an opponent...\n");

  int received_int = 0;

  while (not_found_opponent) {
    /* int random_num = 0; */
    /* int converted_num = htonl(random_num); */

    /* write( sd, &converted_num, sizeof(converted_num) ); */

    read( sd, &received_int, sizeof(received_int) );
    printf( "received: %d\n", received_int );

    if (received_int == -1) {
      not_found_opponent = 0;
    }

  }

  printf("Game started!\n");

  int converted_num = htonl(0);
  write( sd, &converted_num, sizeof(converted_num) );

  read( sd, &received_int, sizeof(received_int) );
  printf( "received: %d\n", received_int );
  
  return 0;
}

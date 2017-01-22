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

  while (1) {
    printf("enter message: ");

    fgets( buffer, sizeof(buffer), stdin );

    char *p = strchr(buffer, '\n');
    *p = '\0';

    int random_num = 5;
    int converted_num = htonl(random_num);

    write( sd, &converted_num, sizeof(converted_num) );

    int received_int = 0;

    read( sd, &received_int, sizeof(received_int) );
    printf( "received: %d\n", received_int );
  }

  return 0;
}

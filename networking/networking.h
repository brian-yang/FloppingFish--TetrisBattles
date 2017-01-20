#ifndef NETWORKING_H
#define NETWORKING_H

#define MESSAGE_BUFFER_SIZE 1000

#include "connection.h"

void error_check( int i, char *s );

int server_setup();
struct connection_info* server_connect(int sd);

int client_connect( char *host );

#endif

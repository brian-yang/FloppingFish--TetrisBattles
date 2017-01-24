#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"
#include "common.h"

game* init_game();

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

  //==========================================
  game *ff;

  WINDOW *board, *next, *hold, *score;
  ff = (game*) init_game();

  int startx, starty, width, height;
  height = 22;
  width = 30;
  starty = (LINES - height) / 2;  /* Calculating for a center placement */
  startx = (COLS - width) / 2;
  // Create windows for each section of the interface.
  board = newwin(ff->rows + 2, 2 * ff->cols + 2, starty, startx);
  next  = newwin(6, 10, starty+2, startx+(2 * (ff->cols + 1) + 1));
  hold  = newwin(6, 10, starty+9, startx+(2 * (ff->cols + 1) + 1));
  score = newwin(6, 10, starty+15, startx+(2 * (ff->cols + 1 ) + 1 ));

  bool running = true;
  int game_status = 0;
  //==========================================

  while (running) {
    /* game_status = run_game(ff, board, next, hold, score); */
    /* if (game_status == -1) { */
    /*   running = false; */
    /* } */

    running = ff_tick(ff);
    display_board(board, ff);
    display_piece(next, ff->next);
    display_piece(hold, ff->stored);
    display_score(score, ff);
    doupdate();
    /* refresh(); */

    //case handling
    switch(getch()){
    case KEY_LEFT:
      ff_move(ff, -1);
      break;
    case KEY_RIGHT:
      ff_move(ff, 1);
      break;
    case KEY_UP:
      ff_rotate(ff, 1);
      break;
    case KEY_DOWN:
      break;
    case 'q':
      running = false;
      break;
    case 'l':
      ff_getline(ff,ff->rows,board);
      break;
    case ' ':
      ff_down(ff);
      break;
    default:
      break;
    }

    int converted_num = htonl(running);
    write(sd, &converted_num, sizeof(converted_num));

    read(sd, &received_int, sizeof(received_int) );
    if (received_int == 0) {
      running = false;
    }

    sleep_milli(15);
  }
  end_game(ff);

  return 0;
}

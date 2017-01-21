#include <unistd.h>
#include "common.h"

#define BRD_TOP    ((LINES - BRD_HEIGHT)/2)
#define BRD_LEFT   ((COLS - BRD_WIDTH)/2)  
#define BRD_HEIGHT 20
#define BRD_WIDTH  10

static struct {
  int score;
  WINDOW *board;
} game;

void new_game(){
  game.score = 0;
  free_board();
  init_board(BRD_HEIGHT, BRD_WIDTH);
  wclear(stdscr);
  wclear(game.board);
}

void init_ncurses() {
  initscr(); //start curses mode
  noecho(); //don't echo keypresses
  cbreak();
  halfdelay(5);
  curs_set(false); //don't display cursor
  keypad(stdscr, true);
  srand(time(NULL));
  if (has_colors()) { //if terminal supports color
    start_color();
    //initialize color pairs
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(BLACK, COLOR_BLACK, COLOR_WHITE);
    init_pair(RED, COLOR_RED, COLOR_RED);
    init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(CYAN, COLOR_CYAN, COLOR_CYAN);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(ORANGE, COLOR_YELLOW, COLOR_YELLOW);
  } else {
    fprintf(stderr, "[Error] Terminal does not support colors. \n");
    endwin();
    exit(1);
  }
}

int main(){
  init_ncurses();
  game.board = newwin(BRD_HEIGHT,BRD_WIDTH,BRD_TOP,BRD_LEFT);
  new_game();
  while(1){
    draw_board(game.board);
  }
  return 0;
}

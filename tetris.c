#include <unistd.h>
#include <ncurses.h>

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
    init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);
  } else {
    fprintf(stderr, "[Error] Terminal does not support colors. \n");
    endwin();
    exit(1);
  }
}

int main(){
  init_ncurses();
  game.board = newwin(BRD_HEIGHT,BRD_WIDTH,BRD_TOP,BRD_LEFT);
  while(route(getch())){
    draw_board(game.board);
  }
  return 0;
}

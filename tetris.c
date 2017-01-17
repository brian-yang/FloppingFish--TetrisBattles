#include <unistd.h>
#include <ncurses.h>

#define W_HEIGHT 20
#define W_WIDTH  10

void initcurses() {
  initscr(); //start curses mode  no
  noecho();
  cbreak();
  halfdelay(5);
  curs_set(false);
  keypad(stdscr, true);
  srand(time(NULL));
  refresh();
}

WINDOW *create_board(int height, int width, int starty, int startx){
  WINDOW *board;
  board = newwin(height, width, starty, startx);
  box(board, 0, 0);
  wrefresh(board);
  return board;
}

int main(){
  WINDOW *board;
  int startx, starty;

  initcurses();
  starty = (LINES - W_HEIGHT)/2;
  startx = (COLS - W_WIDTH)/2 ;
  
  board = create_board(W_HEIGHT, W_WIDTH, starty, startx);
  while(1){
  };
  endwin();
  return 0;
}

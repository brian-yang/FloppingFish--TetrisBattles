#include <unistd.h>
#include <ncurses.h>

#define W_HEIGHT 20
#define W_WIDTH  10

static struct 

void initcurses() {
  initscr(); //start curses mode
  noecho(); //don't echo keypresses
  cbreak();
  halfdelay(5);
  curs_set(false); //don't display cursor
  keypad(stdscr, true);
  srand(time(NULL));
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
  start_color();
  init_pair(1,COLOR_GREEN,COLOR_GREEN);
  wattron(board, COLOR_PAIR(1)); 
  mvwprintw(board,1,1,"xx"); 
  getch();
  while(1){
    wrefresh(board);
  };
  
  /*int x = 0, y = 0;
  initcurses();
  while(1){
    clear();
    mvprintw(y,x,"o");
    refresh();
    usleep(3000);
    x++;
  };
  */
  endwin();
  return 0;
}

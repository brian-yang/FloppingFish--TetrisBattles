#include <unistd.h>
#include <ncurses.h>

#define W_HEIGHT 20
#define W_WIDTH  10

typedef struct {
  int x;
  int y;
} point;

struct piece {
  struct tetrimino *tet;
  struct point pos;
};

struct piece newpc(){
  struct piece pc;
  pc.pos.x = 0;
  pc.pos.y = 0;
  return pc;
}
  
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

void draw_piece(struct piece pc, WINDOW *win, int color){
  int ch = ' ';
  mvwaddch(win, pc.pos.y, pc.pos.x, ch);
  start_color();
  init_pair(1,COLOR_GREEN,COLOR_GREEN);
  wattron(win, COLOR_PAIR(1)); 
  mvwprintw(win,0,30,"xx"); 
}

void drawboard(WINDOW *brd){
  wclear(brd);
  box(brd, false, false);
}

/*void init_board(WINDOW *boardint height, int width){
  board.height = height;
  board.width = width;
  }*/
  

int main(){
  WINDOW *board;
  int startx, starty;
  
  pt->x = 3;
  pt->y = 4;
  struct piece pc;
  piece->pos = pt;
  initcurses();
  starty = (LINES - W_HEIGHT)/2;
  startx = (COLS - W_WIDTH)/2 ;
  
  board = create_board(W_HEIGHT, W_WIDTH, starty, startx);  
  draw_piece(pc,board,COLOR_RED);
  getch();
  while(1){
    wrefresh(board);
  };
  endwin();
  return 0;
}

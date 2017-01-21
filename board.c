#include "common.h"

static struct board { 
  int height;
  int width;
  int **grid;
  bool topped;
  struct piece pc;
  struct piece holdpc;
} board;


// Cycle the current and next piece and check if the board is topped.                      
void cyclepcs(bool holdset){
  board.pc = (holdset ? board.holdpc : newpc());
  board.holdpc = newpc();
  board.pc.pos.y = board.height/2;
  board.pc.pos.x = board.width/2;
  board.holdpc.pos.y = 0;
  board.holdpc.pos.x = 2;
}

void init_board(int height, int width){
  board.height = height;
  board.width = width;
  cyclepcs(false);
  board.topped = false;
  
  //creating a height x width matrix of integers and initializing them at 0
  int i,j;
  board.grid = malloc(board.height * sizeof(int *)); //# of rows
  for (i = 0; i < board.height; i++){
    board.grid[i] = malloc(board.width * sizeof(int)); //# of columns
    for (j = 0; j < board.width; j++)
      board.grid[i][j] = 0;
  }
}

//free board memory
void free_board(){
  if(board.grid){
    int i;
    for (i = 0; i < board.height; i++)
      free(board.grid[i]);
    free(board.grid);
  }
}

void draw_board(WINDOW *brd_win){
  wclear(brd_win);
  box(brd_win, false, false);
  // draw locked pieces
  int i, j, ch;
  for (i = 0; i < board.height; i++) {
    for (j = 0; j < board.width; j++) {
      if (board.grid[i][j] > 0) {
	ch = ' ' | COLOR_PAIR(board.grid[i][j]);
	mvwaddch(brd_win, i+1, j+1, ch); 
      }
    }
  }
  
  //draw the current piece
  drawpc(board.pc, brd_win, board.pc.tet->color, 1);
}

static void lockpc_map(struct piece pc, struct point pt, void *auxdata){
  board.grid[pt.y][pt.x] = pc.tet->color;
}

// Lock the given piece on to the grid.
static void lockpc(struct piece pc){
  mappc(pc, lockpc_map, NULL);
  cyclepcs(true);
}

#include "common.h"

static struct board { 
  int height;
  int width;
  int **grid;
  bool topped;
  struct piece pc;
  struct piece holdpc;
} brd;


// Cycle the current and next piece and check if the board is topped.                      
void cyclepcs(bool holdset){
  brd.pc = (holdset ? brd.holdpc : newpc());
  brd.holdpc = newpc();
  brd.pc.pos.y = brd.height/2;
  brd.pc.pos.x = brd.width/2;
  brd.holdpc.pos.y = 0;
  brd.holdpc.pos.x = 2;
}

void init_board(int height, int width){
  brd.height = height;
  brd.width = width;
  cyclepcs(false);
  brd.topped = false;
  
  //creating a height x width matrix of integers and initializing them at 0
  int i,j;
  brd.grid = malloc(brd.height * sizeof(int *)); //# of rows
  for (i = 0; i < brd.height; i++){
    brd.grid[i] = malloc(brd.width * sizeof(int)); //# of columns
    for (j = 0; j < brd.width; j++)
      brd.grid[i][j] = 0;
  }
}

//free board memory
void free_board(){
  if(brd.grid){
    int i;
    for (i = 0; i < brd.height; i++)
      free(brd.grid[i]);
    free(brd.grid);
  }
}

void draw_board(WINDOW *board, WINDOW *hold){
  wclear(board);
  wclear(hold);
  box(board, false, false);
  box(hold, false, false);
  // draw locked pieces
  int i, j, ch;
  for (i = 0; i < brd.height; i++) {
    for (j = 0; j < brd.width; j++) {
      if (brd.grid[i][j] > 0) {
	ch = ' ' | COLOR_PAIR(brd.grid[i][j]);
	mvwaddch(board, i+1, j+1, ch); 
      }
    }
  }
  
  //draw the current piece
  drawpc(brd.pc, board, brd.pc.tet->color, 1);
}

static void lockpc_map(struct piece pc, struct point pt, void *auxdata){
  brd.grid[pt.y][pt.x] = pc.tet->color;
}

// Lock the given piece on to the grid.
static void lockpc(struct piece pc){
  mappc(pc, lockpc_map, NULL);
  cyclepcs(true);
}

#include "common.h"

static struct board { 
  int height;
  int width;
  int **grid;
  bool topped;
  struct piece pc;
  struct piece holdpc;
} board;

void init_board(int height, int width){
  board.height = height;
  board.width = width;
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
	ch = ' '; 
	mvwaddch(brd_win, i+1, j+1, ch); 
      }
    }
  }
}

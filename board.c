#include "common.h"

struct piece{};

//create the board structure
static struct board{
  int height;
  int width;
  int **grid;
  bool overflow; //checks if the board is filled
  struct piece pc;
  struct piece holdpc;
} brd;

void initbrd(int height, int width){
  brd.height = height;
  brd.width = width;
  brd.overflow = false;

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
void freebrd(){
  if(brd.grid){
    int i;
    for (i = 0; i < brd.height; i++)
      free(brd.grid[i]);
    free(brd.grid);
  }
}

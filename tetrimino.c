#include "common.h"

static struct tetrimino I = { 
  CYAN,
  2,
  { 
    { {-1,0}, {0,0}, {1,0}, {2,0} },
    { {0,-1}, {0,0}, {0,1}, {0,2} }
  } 
};

struct piece newpc(){
  struct piece pc;
  pc.pos.x = 0;
  pc.pos.y = 0;
  pc.rotid = 0;
  pc.tet = &I;
  return pc;
};

void drawpc(struct piece pc, WINDOW *board, int color){
  int ch = ' ';
  mvwaddch(board, pc.y, pc.x, ch);
}


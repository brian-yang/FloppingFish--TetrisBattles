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

struct drawpc_args { 
  WINDOW *win; 
  int color; 
  //int pad; 
};

void mappc(struct piece pc, mappc_fn fn, void *auxdata){
  struct point pt;
  int i;
  for (i=0; i < 4; i++) {
    pt.x = pc.pos.x+pc.tet->rotations[pc.rotid][i].x;
    pt.y = pc.pos.y+pc.tet->rotations[pc.rotid][i].y;
    fn(pc, pt, auxdata);
  }
}

static void drawpc_map(struct piece pc, struct point pt, void *auxdata){
  struct drawpc_args *args = (struct drawpc_args *) auxdata;
  int ch = ' ' | COLOR_PAIR(args->color);
  mvwaddch(args->win, pt.y, pt.x, ch);
}


void drawpc(struct piece pc, WINDOW *win, int color){
  struct drawpc_args args = { win, color};
  mappc(pc, drawpc_map, &args);
}


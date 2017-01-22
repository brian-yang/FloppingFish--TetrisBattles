#include "common.h"

//block declarations
static struct tetrimino I = { 
  CYAN,
  // I
  {{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
   {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
   {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
   {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
};
static struct tetrimino J = {
  BLUE,
  // J
  {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
   {{0, 1}, {1, 1}, {2, 0}, {2, 1}}},
  
};
static struct tetrimino L = {
  ORANGE,
  // L
  {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
   {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
};
static struct tetrimino O = {
  YELLOW,
  // O
  {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}}},
};
static struct tetrimino S  = {
  GREEN,
  // S
  {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
   {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
   {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
};
static struct tetrimino T = {
  MAGENTA,
  // T
  {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 1}}},
};
static struct tetrimino Z = {  
  RED,
  // Z
  {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
   {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 0}}}
};

struct piece newpc(){
  struct piece pc;
  pc.pos.x = 0;
  pc.pos.y = 0;
  pc.rotid = 2;
  pc.tet = &I;
  return pc;
};

struct drawpc_args { 
  WINDOW *win; 
  int color; 
  int pad; 
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
  mvwaddch(args->win, pt.y + args->pad, pt.x + args->pad, ch);
}


void drawpc(struct piece pc, WINDOW *win, int color, int pad){
  struct drawpc_args args = { win, color, pad};
  mappc(pc, drawpc_map, &args);
}


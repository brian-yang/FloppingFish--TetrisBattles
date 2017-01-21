#ifndef TETRIMINO_H
#define TETRIMINO_H

struct point {
  int x;
  int y;
};

struct tetrimino {
  enum colors color;
  int rotcount;
  struct point rotations[4][4];
};

struct piece {
  struct tetrimino *tet;
  struct point pos;
  int rotid;
};

struct piece newpc();

void drawpc(struct piece pc, WINDOW *board, int color);

#endif

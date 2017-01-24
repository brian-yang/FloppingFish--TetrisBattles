#ifndef TETRIS_H
#define TETRIS_H

/*
  Convert a tetromino to corresponding cell.
 */
#define TYPE_TO_CELL(x) ((x)+1)
#define IS_EMPTY(x) ((x) == EMPTY)
#define IS_FILLED(x) (!IS_EMPTY(x))

#define TETRIS 4
#define NUM_TETROMINOS 7
#define NUM_ORIENTATIONS 4
#define COLS_PER_CELL 2

/*
  A row,column pair.
 */
typedef struct {
  int row;
  int col;
} pos;

/*
  Piece information - type, orientation, location.
 */
typedef struct {
  int typ;
  int ori;
  pos loc;
} piece;

/*
  A game object!
 */
typedef struct {
  int rows;
  int cols;
  char *board;
  int points;

  piece falling;
  piece next;
  piece stored;

  int ticks_till_gravity;
} game;

/*
  A "cell" is a 1x1 piece within a tetris board. Used for color initialization.
 */
typedef enum {
  EMPTY, I, J, L, O, S, T, Z
} tetris_cell;

/*
  Array storing tetromino information - type, orientation, and mapping arrays.
 */
extern pos TETROMINOS[NUM_TETROMINOS][NUM_ORIENTATIONS][TETRIS];

/*
  Functions for game behavior and control
*/
void ff_move(game *obj, int direction);
void ff_init(game *obj, int rows, int cols);
void ff_getline(game*obj, int r, WINDOW* board);
game *ff_create(int rows, int cols);
void ff_destroy(game *obj);
void ff_delete(game *obj);
void ff_down(game *obj);
void ff_rotate(game *obj, int direction);
void ff_hold(game *obj);
char ff_get(game *obj, int row, int col);
bool ff_check(game *obj, int row, int col);
bool ff_tick(game *obj);


void sleep_milli(int milliseconds);
#endif

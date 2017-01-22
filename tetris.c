#include "common.h"

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

pos TETROMINOS[NUM_TETROMINOS][NUM_ORIENTATIONS][TETRIS] = {
  // I
  {{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
   {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
   {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
   {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
  // J
  {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
   {{0, 1}, {1, 1}, {2, 0}, {2, 1}}},
  // L
  {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
   {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
  // O
  {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
   {{0, 1}, {0, 2}, {1, 1}, {1, 2}}},
  // S
  {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
   {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
   {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
  // T
  {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
   {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 1}}},
  // Z
  {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
   {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
   {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
   {{0, 1}, {1, 0}, {1, 1}, {2, 0}}},
};

/*
   Return the piece at the given row and column.
 */
char ff_get(game *obj, int row, int column)
{
  return obj->board[obj->cols * row + column];
}

/*
  Set the piece at the given row and column.
 */
static void ff_set(game *obj, int row, int column, char value)
{
  obj->board[obj->cols * row + column] = value;
}

/*
  Boundary checking
 */
bool ff_check(game *obj, int row, int col)
{
  return 0 <= row && row < obj->rows && 0 <= col && col < obj->cols;
}

/*
  Place a piece onto the board.
 */
static void ff_put(game *obj, piece piece)
{
  int i;
  for (i = 0; i < TETRIS; i++) {
    pos cell = TETROMINOS[piece.typ][piece.ori][i];
    ff_set(obj, piece.loc.row + cell.row, piece.loc.col + cell.col,
           TYPE_TO_CELL(piece.typ));
  }
}

/*
  Clear row.
 */
static void ff_remove(game *obj, piece piece)
{
  int i;
  for (i = 0; i < TETRIS; i++) {
    pos cell = TETROMINOS[piece.typ][piece.ori][i];
    ff_set(obj, piece.loc.row + cell.row, piece.loc.col + cell.col, EMPTY);
  }
}

/*
  Checks if piece can be placed on board.
 */
static bool ff_fits(game *obj, piece piece)
{
  int i, r, c;
  for (i = 0; i < TETRIS; i++) {
    pos cell = TETROMINOS[piece.typ][piece.ori][i];
    r = piece.loc.row + cell.row;
    c = piece.loc.col + cell.col;
    if (!ff_check(obj, r, c) || IS_FILLED(ff_get(obj, r, c))) {
      return false;
    }
  }
  return true;
}

/*
  Return a random tetromino type.
 */
static int random_tetromino(void) {
  return rand() % NUM_TETROMINOS;
}

/*
  Initialize new falling piece, populate the next piece with random tetromino.
 */
static void ff_new_falling(game *obj)
{
  // Put in a new falling tetromino.
  obj->falling = obj->next;
  obj->next.typ = random_tetromino();
  obj->next.ori = 0;
  obj->next.loc.row = 0;
  obj->next.loc.col = obj->cols/2 - 2;
}

/*
  Tick gravity, and move the piece down if there is gravity. 
 */
static void ff_do_gravity_tick(game *obj)
{
  obj->ticks_till_gravity--;
  if (obj->ticks_till_gravity <= 0) {
    ff_remove(obj, obj->falling);
    obj->falling.loc.row++;
    if (ff_fits(obj, obj->falling)) {
      obj->ticks_till_gravity = 50;
    } else {
      obj->falling.loc.row--;
      ff_put(obj, obj->falling);

      ff_new_falling(obj);
    }
    ff_put(obj, obj->falling);
  }
}

/*
  Left and right movement.
 */
void ff_move(game *obj, int direction)
{
  ff_remove(obj, obj->falling);
  obj->falling.loc.col += direction;
  if (!ff_fits(obj, obj->falling)) {
    obj->falling.loc.col -= direction;
  }
  ff_put(obj, obj->falling);
}

/*
  Drop piece to bottom. 
 */
void ff_down(game *obj)
{
  ff_remove(obj, obj->falling);
  while (ff_fits(obj, obj->falling)) {
    obj->falling.loc.row++;
  }
  obj->falling.loc.row--;
  ff_put(obj, obj->falling);
  ff_new_falling(obj);
}

/*
  Rotate piece. 
 */
void ff_rotate(game *obj, int direction)
{
  ff_remove(obj, obj->falling);

  while (true) {
    obj->falling.ori = (obj->falling.ori + direction) % NUM_ORIENTATIONS;

    // If the new orientation fits, we're done.
    if (ff_fits(obj, obj->falling))
      break;

    // Otherwise, try moving left to make it fit.
    obj->falling.loc.col--;
    if (ff_fits(obj, obj->falling))
      break;

    // Finally, try moving right to make it fit.
    obj->falling.loc.col += 2;
    if (ff_fits(obj, obj->falling))
      break;

    // Put it back in its original location and try the next orientation.
    obj->falling.loc.col--;
    // Worst case, we come back to the original orientation and it fits, so this
    // loop will terminate.
  }

  ff_put(obj, obj->falling);
}

/*
  Swap the falling piece with the piece in the hold buffer.
 */
void ff_hold(game *obj)
{
  ff_remove(obj, obj->falling);
  if (obj->stored.typ == -1) {
    obj->stored = obj->falling;
    ff_new_falling(obj);
  } else {
    int typ = obj->falling.typ, ori = obj->falling.ori;
    obj->falling.typ = obj->stored.typ;
    obj->falling.ori = obj->stored.ori;
    obj->stored.typ = typ;
    obj->stored.ori = ori;
    while (!ff_fits(obj, obj->falling)) {
      obj->falling.loc.row--;
    }
  }
  ff_put(obj, obj->falling);
}

/*
  Return true if line i is full.
 */
static bool ff_line_full(game *obj, int i)
{
  int j;
  for (j = 0; j < obj->cols; j++) {
    if (IS_EMPTY(ff_get(obj, i, j)))
      return false;
  }
  return true;
}

/*
  Shift every row above r down one.
 */
static void ff_shift_lines(game *obj, int r)
{
  int i, j;
  for (i = r-1; i >= 0; i--) {
    for (j = 0; j < obj->cols; j++) {
      ff_set(obj, i+1, j, ff_get(obj, i, j));
      ff_set(obj, i, j, EMPTY);
    }
  }
}

/*
  Find rows that are filled, remove them, shift, and return the number of
  cleared rows.
 */
static int ff_check_lines(game *obj)
{
  int i, nlines = 0;
  ff_remove(obj, obj->falling); // don't want to mess up falling piece

  for (i = obj->rows-1; i >= 0; i--) {
    if (ff_line_full(obj, i)) {
      ff_shift_lines(obj, i);
      i++; // do this line over again since they're shifted
      nlines++;
    }
  }

  ff_put(obj, obj->falling); // replace
  return nlines;
}

/*
  Adjust the score for the game, given how many lines were just cleared.
 */
static void ff_adjust_score(game *obj, int lines_cleared)
{
  static int line_multiplier[] = {0, 40, 100, 300, 1200};
  obj->points += line_multiplier[lines_cleared];
}

/*
  Return true if the game is over.
 */
static bool ff_game_over(game *obj)
{
  int i, j;
  bool over = false;
  ff_remove(obj, obj->falling);
  for (i = 0; i < 2; i++) {
    for (j = 0; j < obj->cols; j++) {
      if (IS_FILLED(ff_get(obj, i, j))) {
        over = true;
      }
    }
  }
  ff_put(obj, obj->falling);
  return over;
}

/*
  Do a single game tick: process gravity, user input, and score.  Return true if
  the game is still running, false if it is over.
 */
bool ff_tick(game *obj)
{
  int lines_cleared;
  // Handle gravity.
  ff_do_gravity_tick(obj);

  // Check for cleared lines
  lines_cleared = ff_check_lines(obj);

  ff_adjust_score(obj, lines_cleared);

  // Return whether the game will continue 
  return !ff_game_over(obj);
}

void ff_init(game *obj, int rows, int cols)
{
  // Initial setup and variables 
  obj->rows = rows;
  obj->cols = cols;
  obj->board = malloc(rows * cols);
  memset(obj->board, EMPTY, rows * cols);
  obj->points = 0;
  obj->ticks_till_gravity = 50;
  srand(time(NULL));
  ff_new_falling(obj);
  ff_new_falling(obj);
  obj->stored.typ = -1;
  obj->stored.ori = 0;
  obj->stored.loc.row = 0;
  obj->next.loc.col = obj->cols/2 - 2;
}

game *ff_create(int rows, int cols)
{
  game *obj = malloc(sizeof(game));
  ff_init(obj, rows, cols);
  return obj;
}

void ff_destroy(game *obj)
{
  free(obj->board);
}

void ff_delete(game *obj) 
{
  ff_destroy(obj);
  free(obj);
}

void sleep_milli(int milliseconds)
{
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = milliseconds * 1000 * 1000;
  nanosleep(&ts, NULL);
}








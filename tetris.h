#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h> // for FILE
#include <stdbool.h> // for bool

/*
  Convert a tetromino type to its corresponding cell.
 */
#define TYPE_TO_CELL(x) ((x)+1)

/*
  Strings for how you would print a tetris board.
 */
#define TC_EMPTY_STR " "
#define TC_BLOCK_STR "\u2588"

/*
  Questions about a tetris cell.
 */
#define TC_IS_EMPTY(x) ((x) == TC_EMPTY)
#define TC_IS_FILLED(x) (!TC_IS_EMPTY(x))

#define TETRIS 4
#define NUM_TETROMINOS 7
#define NUM_ORIENTATIONS 4

/*
  A row,column pair.
 */
typedef struct {
  int row;
  int col;
} tetris_location;

/*
  Block information - type, orientaiton, location
 */
typedef struct {
  int typ;
  int ori;
  tetris_location loc;
} tetris_block;

/*
  A game object!
 */
typedef struct {
  int rows;
  int cols;
  char *board;
  int points;
  
  tetris_block falling;
  tetris_block next;
  tetris_block stored;
  
  int ticks_till_gravity;
} tetris_game;

/*
  Array storing tetromino information - type, orientation, and mapping arrays
 */
extern tetris_location TETROMINOS[NUM_TETROMINOS][NUM_ORIENTATIONS][TETRIS];

// Data structure manipulation.
void tg_move(tetris_game *obj, int direction);
void tg_init(tetris_game *obj, int rows, int cols);
tetris_game *tg_create(int rows, int cols);
void tg_destroy(tetris_game *obj);
void tg_delete(tetris_game *obj);
tetris_game *tg_load(FILE *f);
void tg_save(tetris_game *obj, FILE *f);
void tg_down(tetris_game *obj);
void tg_rotate(tetris_game *obj, int direction);
void tg_hold(tetris_game *obj);
char tg_get(tetris_game *obj, int row, int col);
bool tg_check(tetris_game *obj, int row, int col);
bool tg_tick(tetris_game *obj);
void sleep_milli(int milliseconds);
#endif

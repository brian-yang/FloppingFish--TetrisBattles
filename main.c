#include "common.h"

/*
  2 columns per cell makes the game much nicer.
*/
#define COLS_PER_CELL 2
/*
  Macro to print a cell of a specific type to a window.
*/
#define ADD_BLOCK(w,x) waddch((w),' '|A_REVERSE|COLOR_PAIR(x));     \
  waddch((w),' '|A_REVERSE|COLOR_PAIR(x))
#define ADD_EMPTY(w) waddch((w), ' '|A_REVERSE|COLOR_PAIR(8)); waddch((w), ' '|A_REVERSE|COLOR_PAIR(8))

/*
  Print the tetris board onto the ncurses window.
*/
void display_board(WINDOW *w, tetris_game *obj)
{
  int i, j;
  box(w, 0, 0);
  for (i = 0; i < obj->rows; i++) {
    wmove(w, 1 + i, 1);
    for (j = 0; j < obj->cols; j++) {
      if (TC_IS_FILLED(tg_get(obj, i, j))) {
        ADD_BLOCK(w,tg_get(obj, i, j));
      } else {
        ADD_EMPTY(w);
      }
    }
  }
  wnoutrefresh(w);
}

/*
  Display a tetris piece in a dedicated window.
*/
void display_piece(WINDOW *w, tetris_block block)
{
  int b;
  pos c;
  wclear(w);
  box(w, 0, 0);
  if (block.typ == -1) {
    wnoutrefresh(w);
    return;
  }
  for (b = 0; b < TETRIS; b++) {
    c = TETROMINOS[block.typ][block.ori][b];
    wmove(w, c.row + 1, c.col * COLS_PER_CELL + 1);
    ADD_BLOCK(w, TYPE_TO_CELL(block.typ));
  }
  wnoutrefresh(w);
}

/*
  Display score information in a dedicated window.
*/
void display_score(WINDOW *w, tetris_game *tg)
{
  wclear(w);
  box(w, 0, 0);
  wprintw(w, "Score\n%d\n", tg->points);
  wnoutrefresh(w);
}

/*
  Do the NCURSES initialization steps for color blocks.
*/
void init_colors(void)
{
  start_color();
use_default_colors();

  init_pair(TC_CELLI, COLOR_CYAN, COLOR_BLACK);
  init_pair(TC_CELLJ, COLOR_BLUE, COLOR_BLACK);
  init_pair(TC_CELLL, COLOR_WHITE, COLOR_BLACK);
  init_pair(TC_CELLO, COLOR_YELLOW, COLOR_BLACK);
  init_pair(TC_CELLS, COLOR_GREEN, COLOR_BLACK);
  init_pair(TC_CELLT, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(TC_CELLZ, COLOR_RED, COLOR_BLACK);
}

/*
  Main tetris game!
*/
int main(int argc, char **argv)
{
  tetris_game *tg;
  bool running = true;

  WINDOW *board, *next, *hold, *score;
  tg = tg_create(22, 10);
  // NCURSES initialization:
  initscr();             // initialize curses
  cbreak();              // pass key presses to program, but not signals
  noecho();              // don't echo key presses to screen
  keypad(stdscr, TRUE);  // allow arrow keys
  timeout(0);            // no blocking on getch()
  curs_set(0);           // set the cursor to invisible
  init_colors();         // setup tetris colors
  bkgdset(COLOR_PAIR(4));
  int startx, starty, width, height;
  height = 22;
  width = 30;
  starty = (LINES - height) / 2;  /* Calculating for a center placement */
  startx = (COLS - width) / 2;  /* of the window    */
  // Create windows for each section of the interface.
  board = newwin(tg->rows + 2, 2 * tg->cols + 2, starty, startx);
  next  = newwin(6, 10, starty+0, startx+(2 * (tg->cols + 1) + 1));
  hold  = newwin(6, 10, starty+7, startx+(2 * (tg->cols + 1) + 1));
  score = newwin(6, 10, starty+14, startx+(2 * (tg->cols + 1 ) + 1));

  // Game loop
  while (running) {
    running = tg_tick(tg);
    display_board(board, tg);
    display_piece(next, tg->next);
    display_piece(hold, tg->stored);
    display_score(score, tg);
    /* doupdate(); */
    refresh();
    switch(getch()){
    case KEY_LEFT:
      tg_move(tg, -1);
      break;
    case KEY_RIGHT:
      tg_move(tg, 1);
      break;
    case KEY_UP:
      tg_rotate(tg, 1);
      break;
    case KEY_DOWN:
      break;
    case 'q':
	running = false;
	break;
    case ' ':
      tg_down(tg);
      break;
    default:
      break;
    }
        sleep_milli(7);
  }


  // Deinitialize NCurses
  wclear(stdscr);
  endwin();

  // Output ending message.
  printf("Game over!\n");

  // Deinitialize Tetris
  tg_delete(tg);
  return 0;
}

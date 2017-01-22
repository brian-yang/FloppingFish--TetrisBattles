#include "common.h"
/*
  Print out individual cells
*/
#define ADD_piece(w,x) waddch((w),' '|A_REVERSE|COLOR_PAIR(x));     \
  waddch((w),' '|A_REVERSE|COLOR_PAIR(x))
#define ADD_EMPTY(w) waddch((w), ' '|A_REVERSE|COLOR_PAIR(8)); waddch((w), ' '|A_REVERSE|COLOR_PAIR(8))

/*
  Print the tetris board
*/
void display_board(WINDOW *w, game *obj)
{
  int i, j;
  box(w, 0, 0);
  for (i = 0; i < obj->rows; i++) {
    wmove(w, 1 + i, 1);
    for (j = 0; j < obj->cols; j++) {
      if (IS_FILLED(ff_get(obj, i, j))) {
        ADD_piece(w,ff_get(obj, i, j));
      } else {
        ADD_EMPTY(w);
      }
    }
  }
  wnoutrefresh(w);
}

/*
  Display a tetris piece
*/
void display_piece(WINDOW *w, piece piece)
{
  int b;
  pos c;
  wclear(w);
  box(w, 0, 0);
  if (piece.typ == -1) {
    wnoutrefresh(w);
    return;
  }
  for (b = 0; b < TETRIS; b++) {
    c = TETROMINOS[piece.typ][piece.ori][b];
    wmove(w, c.row + 1, c.col * COLS_PER_CELL + 1);
    ADD_piece(w, TYPE_TO_CELL(piece.typ));
  }
  wnoutrefresh(w);
}

/*
  Display score information
*/
void display_score(WINDOW *w, game *ff)
{
  wclear(w);
  box(w, 0, 0);
  wprintw(w, "Score\n%d\n", ff->points);
  wnoutrefresh(w);
}

/*
  Ncurses initialization for color pieces
*/
void init_colors(void)
{
  start_color();
use_default_colors();

  init_pair(I, COLOR_CYAN, COLOR_BLACK);
  init_pair(J, COLOR_BLUE, COLOR_BLACK);
  init_pair(L, COLOR_WHITE, COLOR_BLACK);
  init_pair(O, COLOR_YELLOW, COLOR_BLACK);
  init_pair(S, COLOR_GREEN, COLOR_BLACK);
  init_pair(T, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(Z, COLOR_RED, COLOR_BLACK);
}

/*
  Main tetris game! ff for FLOPPING_FISH
*/
int main(int argc, char **argv)
{
  game *ff;
  bool running = true;

  WINDOW *board, *next, *hold, *score;
  ff = ff_create(22, 10);
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
  startx = (COLS - width) / 2;
  // Create windows for each section of the interface.
  board = newwin(ff->rows + 2, 2 * ff->cols + 2, starty, startx);
  next  = newwin(6, 10, starty+0, startx+(2 * (ff->cols + 1) + 1));
  hold  = newwin(6, 10, starty+7, startx+(2 * (ff->cols + 1) + 1));
  score = newwin(6, 10, starty+14, startx+(2 * (ff->cols + 1 ) + 1));

  // Game loop
  while (running) {
    running = ff_tick(ff);
    display_board(board, ff);
    display_piece(next, ff->next);
    display_piece(hold, ff->stored);
    display_score(score, ff);
    /* doupdate(); */
    refresh();

    //case handling
    switch(getch()){
    case KEY_LEFT:
      ff_move(ff, -1);
      break;
    case KEY_RIGHT:
      ff_move(ff, 1);
      break;
    case KEY_UP:
      ff_rotate(ff, 1);
      break;
    case KEY_DOWN:
      break;
    case 'q':
	    running = false;
	    break;
    case ' ':
      ff_down(ff);
      break;
    default:
      break;
    }
    sleep_milli(7);
  }

  //End game message and exit curses mode
  wclear(stdscr);
  endwin();
  printf("Game over!\n");
  ff_delete(ff);
  return 0;
}

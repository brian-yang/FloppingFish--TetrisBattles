#include "common.h"

/*
  Print the tetris board
*/
void display_board(WINDOW *w, game *obj)
{
  int i, j;
  wattron(w,COLOR_PAIR(15));
  box(w, 0 ,0);
  wattroff(w,COLOR_PAIR(15));
  for (i = 0; i < obj->rows ; i++) {
    wattron(w,COLOR_BLACK);
    mvwaddch(w,0,i, ' ');
    wattroff(w,COLOR_BLACK);
  }
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

  //box(w, 0, 0);
  wattron(w,COLOR_PAIR(5));
  mvwprintw(w, 2,1, "       \n" /*,tg->points*/);
  mvwprintw(w, 3,1, " Score \n" /*,tg->points*/);
  mvwprintw(w, 4,1, "   %d   \n", ff->points);
  mvwprintw(w, 4,10, "");
  mvwprintw(w, 5,1, "       \n" /*,tg->points*/);
  wattron(w, COLOR_PAIR(5));

  wattron(w,COLOR_PAIR(10));
  //mvwprintw(w, 4,9, " ");
  wattroff(w, COLOR_PAIR(10));

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
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(15, COLOR_MAGENTA, COLOR_CYAN);
}

game* init_game() {
  game *ff;
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

  return ff;
}

int run_game(game* ff, WINDOW* board, WINDOW* next, WINDOW* hold, WINDOW* score) {
  bool running = ff_tick(ff);
  display_board(board, ff);
  display_piece(next, ff->next);
  display_piece(hold, ff->stored);
  display_score(score, ff);
  doupdate();
  /* refresh(); */

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

  if (!running) {
    return -1;
  }
  return 0;

}

void end_game(game* ff) {
  //End game message and exit curses mode
  wclear(stdscr);
  endwin();
  printf("Game over!\n");
  ff_delete(ff);
}

/* /\* */
/*   Main tetris game! ff for FLOPPING_FISH */
/* *\/ */
/* int main(int argc, char **argv) */
/* { */
/*   game *ff; */

/*   WINDOW *board, *next, *hold, *score; */
/*   ff = init_game(); */

/*   int startx, starty, width, height; */
/*   height = 22; */
/*   width = 30; */
/*   starty = (LINES - height) / 2;  /\* Calculating for a center placement *\/ */
/*   startx = (COLS - width) / 2; */
/*   // Create windows for each section of the interface. */
/*   board = newwin(ff->rows + 2, 2 * ff->cols + 2, starty, startx); */
/*   next  = newwin(6, 10, starty+2, startx+(2 * (ff->cols + 1) + 1)); */
/*   hold  = newwin(6, 10, starty+9, startx+(2 * (ff->cols + 1) + 1)); */
/*   score = newwin(6, 10, starty+15, startx+(2 * (ff->cols + 1 ) + 1 )); */

/*   bool running = true; */
/*   while (running) { */
/*     running = run_game(ff, board, next, hold, score); */
/*     sleep_milli(15); */
/*   } */
/*   end_game(ff); */

/*   return 0; */
/* } */

#include <unistd.h>
#include "common.h"

//defining board structure 
#define BRD_TOP     ((LINES - BRD_HEIGHT)/2)
#define BRD_LEFT    ((COLS - BRD_WIDTH)/2)
#define BRD_HEIGHT  20
#define BRD_WIDTH   10
#define HOLD_HEIGHT 3
#define HOLD_WIDTH  6
#define SIDE_HEIGHT 17
#define SIDE_WIDTH  25
#define PAD         2

static struct{
  int score;
  bool ended;
  WINDOW *brdwin; /* window for the board */
  WINDOW *holdwin; /* window for when pieces are held */
  WINDOW *sidewin; /* window for the side displays */
  PANEL *brdpan;
  PANEL *holdpan;
  PANEL *sidepan;
}game;

//initialize a new game
void newgame(){
  game.score = 0;
  game.ended = false;
  freebrd();
  initbrd(BRD_HEIGHT, BRD_WIDTH);
  wclear(stdscr);
  wclear(game.brdwin);
  wclear(game.holdwin);
  wclear(game.sidewin);
}

//initialize the ncurses function
void initncurses(){
  initscr();  //Starts curses mode
  noecho();   //Don't echo while we do getch
  cbreak();   //Disable line buffering 
  halfdelay(5); //.5 second to receive user input
  keypad(stdscr, TRUE); //allows for extraneous keyboard key (F1,F2..)
  curs_set(false);
  srand(time(NULL));

  if (has_colors()){ //checks if your terminal supports color
    start_color();
    //initialize color pairs
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(BLACK, COLOR_BLACK, COLOR_WHITE);
    init_pair(RED, COLOR_RED, COLOR_RED);
    init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(CYAN, COLOR_CYAN, COLOR_CYAN);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(ORANGE, COLOR_YELLOW, COLOR_YELLOW);
  } else {
    fprintf(stderr, "[Error] Your terminal does not support colors. :(\n");
    endwin();
    exit(1);
  }
}

int main(){
  initncurses();
  game.brdwin = newwin(BRD_HEIGHT+PAD, BRD_WIDTH+PAD, BRD_TOP, BRD_LEFT);
  game.holdwin = newwin(HOLD_HEIGHT+PAD, HOLD_WIDTH+PAD, BRD_TOP, BRD_LEFT+BRD_WIDTH+2);
  game.sidewin = newwin(SIDE_HEIGHT+PAD, SIDE_WIDTH+PAD, BRD_TOP+HOLD_HEIGHT+2, BRD_LEFT+BRD_WIDTH+3);
  game.brdpan = new_panel(game.brdwin);
  game.holdpan = new_panel(game.holdwin);
  game.sidepan = new_panel(game.sidewin);
  newgame();
}

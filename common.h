#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "tetris.h"

/*
  Print out individual cells
*/
#define ADD_piece(w,x) waddch((w),' '|A_REVERSE|COLOR_PAIR(x));	\
  waddch((w),' '|A_REVERSE|COLOR_PAIR(x))
#define ADD_EMPTY(w) wattron(w,COLOR_BLACK); waddch((w), ' '); waddch((w), ' '); wattroff(w,COLOR_BLACK);

#endif

#include <ncurses.h>
#include <unistd.h>

#include "../../brickgame/tetris/tetris.h"

#define WINDOW_WIDTH 44
#define WINDOW_HEIGHT 28

#define WINDOW_X 0
#define WINDOW_Y 0

void init_colors();
void print_game_intr(WINDOW *win);
void print_board(WINDOW *win, WINDOW *board, game_info *gi);
void print_score(WINDOW *score_board, game_info *gi);
void print_next_figure(WINDOW *nf_board, figure *next_fig);
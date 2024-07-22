#ifndef BRICKGAME_H
#define BRICKGAME_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define WIDTH 10
#define HEIGHT 21
#define TYPE_NUMBER 7
#define FIG_SIZE 4
#define BLOCK_NUMBER 4
#define START_SPEED 50
#define SPEED_STEP 5


typedef enum {
  Start = 's',
  Pause = 'p',
  Terminate = 'q',
  Left = KEY_LEFT,
  Right = KEY_RIGHT,
  Up,
  Down = KEY_DOWN,
  Action = KEY_UP
} UserAction_t;

typedef enum {
  Stick,
  Left_L,
  Right_L,
  Cube,
  Right_Z,
  Fuck,
  Left_Z,
  Empty
} FigureType_t;

typedef struct GameInfo_t {
  int** field;
  int** next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} game_info;

typedef struct Figure_t {
  int type;
  int spawn_x;
  int spawn_y;
  int** figure;
} figure;

typedef struct BlockLocation_t {
  int x;
  int y;
} block;

game_info* get_game_info_instance();
figure* get_current_figure();
figure* get_next_figure();
int* get_tick();
void ncurses_start();
void ncurses_end(WINDOW*board, WINDOW*score_board,WINDOW*nf_board);
int random_number();

void init_game_info(game_info* gi);
void remove_field(int** field);
void copy_field(int** src, int** dst);

void fill_figure_matrix(figure* fig);
void remove_figure(int** figure);
void init_new_figure(figure* fig);
void update_figures(figure* fig, figure* next_fig);

void spawn_figure(figure fig, int** field);
void despawn_figure(figure fig, int** field);
void rotate_figure(figure* fig, int** field);
void rotate_figure_back(figure* fig, int** field);
void figure_move_left(figure* fig, int** field);
void figure_move_right(figure* fig, int** field);
void figure_move_down(figure* fig, figure* next_fig, int** field);
block get_block_location(figure fig, int block_number);
void user_input(UserAction_t action, bool hold);

game_info update_current_state();

int collision(figure fig, int** field);

int line_is_filled(int** field, int row_number);
int line_is_empty(int** field, int row_number);
int remove_filled_rows(int** field);
void delete_row(int** field, int row_number);
int calc_score(game_info* gi, int field_lines);
void write_high_score(game_info* gi);
void read_high_score(game_info* gi);
int is_game_over(game_info* gi, figure* fig, figure* next_fig);

#endif
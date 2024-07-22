#include "cli.h"

void init_colors() {
    start_color();
    init_pair(Stick, COLOR_RED, COLOR_BLACK);
    init_pair(Left_L, COLOR_CYAN, COLOR_BLACK);
    init_pair(Right_L, COLOR_YELLOW, COLOR_BLACK);
    init_pair(Cube, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(Right_Z, COLOR_GREEN, COLOR_BLACK);
    init_pair(Fuck, COLOR_BLUE, COLOR_BLACK);
    init_pair(Left_Z, COLOR_WHITE, COLOR_BLACK);
}

void print_board(WINDOW *win, WINDOW *board, game_info *gi) {
    box(win, 0, 0);
    wrefresh(win);

    print_game_intr(win);

    wbkgd(board, ' ');
    int add = 1;
    for (int i = 1; i < HEIGHT; i++) {
        if (i == 1) add = 0;
        wmove(board, i + add, 2);
        for (int j = 0; j < WIDTH; j++) {
            if (gi->field[i][j] != Empty) {
                waddch(board, ' ' | A_REVERSE | COLOR_PAIR(gi->field[i][j]));
                waddch(board, ' ' | A_REVERSE | COLOR_PAIR(gi->field[i][j]));
            } else {
                waddch(board, ' ');
                waddch(board, ' ');
            }
        }
    }
    box(board, 0, 0);
    wnoutrefresh(board);
}

void print_game_intr(WINDOW *win) {
    wmove(win, 3, 30);
    wprintw(win, "Next figure");
    wmove(win, 2, 10);
    wprintw(win, "<<<TETRIS>>>");
    wmove(win, 16, 31);
    wprintw(win, "Game Control");
    wmove(win, 17, 31);
    wprintw(win, "[S]  Start");
    wmove(win, 18, 31);
    wprintw(win, "[Q]  Game Quit");
    wmove(win, 19, 31);
    wprintw(win, "[P]  Pause");
    wmove(win, 20, 31);
    wprintw(win, "[->] Left");
    wmove(win, 21, 31);
    wprintw(win, "[<-] Right");
    wmove(win, 22, 31);
    wprintw(win, "[|]  Turn");
}

void print_score(WINDOW *score_win, game_info *gi) {
    wmove(score_win, 0, 0);
    wprintw(score_win, "High Score: %d", gi->high_score);
    wmove(score_win, 1, 0);
    wprintw(score_win, "Your Score: %d", gi->score);
    wmove(score_win, 2, 0);
    wprintw(score_win, "Level: %d", gi->level);
    wmove(score_win, 3, 0);
    wprintw(score_win, "Speed: %d", gi->speed);
    wnoutrefresh(score_win);
}


void print_next_figure(WINDOW *nf_board, figure *next_fig) {
    wbkgd(nf_board, ' ');

    for (int i = 0; i < 4; i++) {
        wmove(nf_board, i + 1, 1);
        for (int j = 0; j < 4; j++) {
            if (next_fig->figure[i][j]) {
                waddch(nf_board, ' ' | A_REVERSE | COLOR_PAIR(next_fig->type));
                waddch(nf_board, ' ' | A_REVERSE | COLOR_PAIR(next_fig->type));
            } else {
                waddch(nf_board, ' ');
                waddch(nf_board, ' ');
            }
        }
        box(nf_board, 0, 0);
    }
    wnoutrefresh(nf_board);
}

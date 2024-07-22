#include "brickgame/tetris/tetris.h"
#include "gui/cli/cli.h"

int main() {
    bool playing = true;
    figure *fig = get_current_figure(), *next_fig = get_next_figure();
    WINDOW *board, *score_board, *nf_board, *win;
    ncurses_start();
    win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH + 4, WINDOW_Y, WINDOW_X);
    board = newwin(HEIGHT + 1, 2 * WIDTH + 3, 4, 4);
    nf_board = newwin(6, 10, 4, 30);
    score_board = newwin(8, 20, 10, 30);


    while (playing) {
        init_game_info(get_game_info_instance());
        game_info *gi = get_game_info_instance();
        init_new_figure(fig);
        init_new_figure(next_fig);
        spawn_figure(*fig, gi->field);
        while (true) {
            print_board(win, board, gi);
            print_score(score_board, gi);
            print_next_figure(nf_board, next_fig);
            doupdate();
            usleep(1000000 / 60);
            *gi = update_current_state();
            user_input(getch(), false);
            if (gi->pause == 3) {
                playing = false;
                break;
            } else if (gi->pause == 2) {
                print_board(win, board, gi);
                break;
            }
        }
    }
    remove_figure(fig->figure);
    remove_figure(next_fig->figure);
    remove_field(get_game_info_instance()->field);
    remove_field(get_game_info_instance()->next);
    ncurses_end(board, score_board, nf_board);
    return 0;
}


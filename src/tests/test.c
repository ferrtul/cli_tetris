#include "test.h"

START_TEST(get_game_info_instance_test) {
        game_info * gi1 = get_game_info_instance();
        game_info *gi2 = get_game_info_instance();

        ck_assert_ptr_nonnull(gi1);
        ck_assert_ptr_eq(gi1, gi2);
}

END_TEST

START_TEST(get_current_figure_test) {
    figure *fig1 = get_current_figure();
    figure *fig2 = get_current_figure();

    ck_assert_ptr_nonnull(fig1);
    ck_assert_ptr_eq(fig1, fig2);
}

END_TEST

START_TEST(get_next_figure_test) {
    figure *fig1 = get_next_figure();
    figure *fig2 = get_next_figure();

    ck_assert_ptr_nonnull(fig1);
    ck_assert_ptr_eq(fig1, fig2);
}

END_TEST

START_TEST(get_tick_test) {
    int *tick1 = get_tick();
    int *tick2 = get_tick();

    ck_assert_ptr_nonnull(tick1);
    ck_assert_ptr_eq(tick1, tick2);
}

END_TEST

START_TEST(random_number_test) {
    int occurrences[7] = {0};
    for (int i = 0; i < 1000; ++i) {
        int num = random_number();
        ck_assert_int_ge(num, 0);
        ck_assert_int_le(num, 6);
        occurrences[num]++;
    }

    for (int i = 0; i < 7; ++i) {
        ck_assert_int_ge(occurrences[i], 100);  // Нижний предел (прим.: можно изменить по необходимости)
        ck_assert_int_le(occurrences[i], 200);  // Верхний предел (прим.: можно изменить по необходимости)
    }
}

END_TEST

START_TEST(init_game_info_test) {
    game_info gi;
    init_game_info(&gi);

    ck_assert_ptr_nonnull(gi.field);
    ck_assert_ptr_nonnull(gi.next);

    for (int i = 0; i < HEIGHT; i++) {
        ck_assert_ptr_nonnull(gi.field[i]);
        for (int j = 0; j < WIDTH; j++) {
            ck_assert_int_eq(gi.field[i][j], Empty);
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        ck_assert_ptr_nonnull(gi.next[i]);
        for (int j = 0; j < WIDTH; j++) {
            ck_assert_int_eq(gi.next[i][j], Empty);
        }
    }
    ck_assert_int_eq(gi.score, 0);
    ck_assert_int_eq(gi.level, 1);
    ck_assert_int_eq(gi.pause, 1);
    ck_assert_int_eq(gi.speed, START_SPEED);
}

END_TEST

START_TEST(remove_field_test) {
    int **test_field = (int **) malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++) {
        test_field[i] = (int *) malloc(WIDTH * sizeof(int));
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            test_field[i][j] = i * WIDTH + j;
        }
    }

    remove_field(test_field);
    ck_assert_ptr_null(test_field);
}

END_TEST

START_TEST(copy_field_test) {
    int **src = (int **) malloc(HEIGHT * sizeof(int *));
    int **dst = (int **) malloc(HEIGHT * sizeof(int *));

    for (int i = 0; i < HEIGHT; i++) {
        src[i] = (int *) malloc(WIDTH * sizeof(int));
        dst[i] = (int *) malloc(WIDTH * sizeof(int));
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            src[i][j] = i * WIDTH + j;
        }
    }

    copy_field(src, dst);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            ck_assert_int_eq(dst[i][j], src[i][j]);
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        free(src[i]);
        free(dst[i]);
    }
    free(src);
    free(dst);
}

END_TEST

START_TEST(init_new_figure_test) {
    figure fig;
    init_new_figure(&fig);
    ck_assert_int_ge(fig.type, 0);
    ck_assert_int_lt(fig.type, 7);

    ck_assert_int_eq(fig.spawn_x, WIDTH / 2 - 2);
    ck_assert_int_eq(fig.spawn_y, 0);

    ck_assert_ptr_nonnull(fig.figure);

    for (int i = 0; i < FIG_SIZE; i++) {
        ck_assert_ptr_nonnull(fig.figure[i]);
        for (int j = 0; j < FIG_SIZE; j++) {
            ck_assert_int_eq(fig.figure[i][j], 0);
        }
    }

    for (int i = 0; i < FIG_SIZE; i++) {
        free(fig.figure[i]);
    }
    free(fig.figure);
}

END_TEST

START_TEST(remove_figure_test) {
    int **test_figure = (int **) malloc(FIG_SIZE * sizeof(int *));
    for (int i = 0; i < FIG_SIZE; i++) {
        test_figure[i] = (int *) malloc(FIG_SIZE * sizeof(int));
    }

    for (int i = 0; i < FIG_SIZE; i++) {
        for (int j = 0; j < FIG_SIZE; j++) {
            test_figure[i][j] = i * FIG_SIZE + j;
        }
    }

    remove_figure(test_figure);
    ck_assert_ptr_null(test_figure);
}

END_TEST

START_TEST(update_figures_test) {
    figure fig;
    figure next_fig;

    init_new_figure(&next_fig);
    update_figures(&fig, &next_fig);

    ck_assert_int_eq(fig.type, next_fig.type);
    ck_assert_int_eq(fig.spawn_x, next_fig.spawn_x);
    ck_assert_int_eq(fig.spawn_y, next_fig.spawn_y);

    for (int i = 0; i < FIG_SIZE; i++) {
        for (int j = 0; j < FIG_SIZE; j++) {
            ck_assert_int_eq(fig.figure[i][j], next_fig.figure[i][j]);
        }
    }
    remove_figure(fig.figure);
    remove_figure(next_fig.figure);
}

END_TEST

START_TEST(fill_figure_matrix_test) {
    figure fig;

    fig.figure = (int **) malloc(FIG_SIZE * sizeof(int *));
    for (int i = 0; i < FIG_SIZE; i++) {
        fig.figure[i] = (int *) malloc(FIG_SIZE * sizeof(int));
    }

    for (int type = Stick; type <= Left_Z; type++) {
        fig.type = type;

        fill_figure_matrix(&fig);

        switch (fig.type) {
            case Stick:
                ck_assert_int_eq(fig.figure[1][0], 1);
                ck_assert_int_eq(fig.figure[1][1], 1);
                ck_assert_int_eq(fig.figure[1][2], 1);
                ck_assert_int_eq(fig.figure[1][3], 1);
                break;
            case Left_L:
                ck_assert_int_eq(fig.figure[0][0], 1);
                ck_assert_int_eq(fig.figure[1][0], 1);
                ck_assert_int_eq(fig.figure[1][1], 1);
                ck_assert_int_eq(fig.figure[1][2], 1);
                break;
            case Right_L:
                ck_assert_int_eq(fig.figure[0][2], 1);
                ck_assert_int_eq(fig.figure[1][0], 1);
                ck_assert_int_eq(fig.figure[1][1], 1);
                ck_assert_int_eq(fig.figure[1][2], 1);
                break;
            case Cube:
                ck_assert_int_eq(fig.figure[1][1], 1);
                ck_assert_int_eq(fig.figure[1][2], 1);
                ck_assert_int_eq(fig.figure[2][1], 1);
                ck_assert_int_eq(fig.figure[2][2], 1);
                break;
            case Right_Z:
                ck_assert_int_eq(fig.figure[1][1], 1);
                ck_assert_int_eq(fig.figure[1][2], 1);
                ck_assert_int_eq(fig.figure[2][0], 1);
                ck_assert_int_eq(fig.figure[2][1], 1);
                break;
            case Fuck:
                ck_assert_int_eq(fig.figure[1][1], 1);
                ck_assert_int_eq(fig.figure[2][0], 1);
                ck_assert_int_eq(fig.figure[2][1], 1);
                ck_assert_int_eq(fig.figure[2][2], 1);
                break;
            case Left_Z:
                ck_assert_int_eq(fig.figure[1][0], 1);
                ck_assert_int_eq(fig.figure[1][1], 1);
                ck_assert_int_eq(fig.figure[2][1], 1);
                ck_assert_int_eq(fig.figure[2][2], 1);
                break;
            default:
                ck_abort_msg("Unknown figure type");
                break;
        }

        for (int i = 0; i < FIG_SIZE; i++) {
            for (int j = 0; j < FIG_SIZE; j++) {
                fig.figure[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < FIG_SIZE; i++) {
        free(fig.figure[i]);
    }
    free(fig.figure);
}

END_TEST

        Suite
*

test_brick_game_tetris() {
    Suite * s = suite_create("\033[45m\ttest_brickgame_tetris\t\t\033[0m");
    TCase *o = tcase_create("test_brickgame_tetris");
    suite_add_tcase(s, o);


    tcase_add_test(o, get_game_info_instance_test());

    return s;
}
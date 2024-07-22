#include "tetris.h"

game_info *get_game_info_instance() {
    static game_info gi_inst;
    return &gi_inst;
}

figure *get_current_figure() {
    static figure cur_fig;
    return &cur_fig;
}

figure *get_next_figure() {
    static figure cur_fig;
    return &cur_fig;
}

int *get_tick() {
    static int tick;
    return &tick;
}

void ncurses_start() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    timeout(0);
    curs_set(0);
}

void ncurses_end(WINDOW *board, WINDOW *score_board, WINDOW *nf_board) {
    wclear(stdscr);
    delwin(board);
    delwin(score_board);
    delwin(nf_board);
    endwin();
}

int random_number() {
    srand(time(NULL));
    int rand_num = rand() % 8;
    if (rand_num != 0) {
        rand_num--;
    }
    return rand_num;
}

void init_game_info(game_info *gi) {
    gi->field = (int **) malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++) {
        gi->field[i] = (int *) malloc(HEIGHT * sizeof(int));
        for (int j = 0; j < WIDTH; j++) gi->field[i][j] = Empty;
    }
    gi->next = (int **) malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++) {
        gi->next[i] = (int *) malloc(WIDTH * sizeof(int));
        for (int j = 0; j < WIDTH; j++) gi->next[i][j] = Empty;
    }

    gi->score = 0;
    read_high_score(gi);
    gi->level = 1;
    gi->pause = 1;
    gi->speed = START_SPEED;
}

void remove_field(int **field) {
    if (field) {
        for (int i = 0; i < HEIGHT; i++) free(field[i]);
        free(field);
        field = NULL;
    }
}

void copy_field(int **src, int **dst) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) dst[i][j] = src[i][j];
    }
}

void init_new_figure(figure *fig) {
    fig->type = random_number();
    fig->spawn_x = WIDTH / 2 - 2;
    fig->spawn_y = 0;
    fig->figure = (int **) malloc(FIG_SIZE * sizeof(int *));
    for (int i = 0; i < FIG_SIZE; i++) {
        fig->figure[i] = (int *) malloc(FIG_SIZE * sizeof(int));
        for (int j = 0; j < FIG_SIZE; j++) fig->figure[i][j] = 0;
    }
    fill_figure_matrix(fig);
}

void remove_figure(int **figure) {
    if (figure) {
        for (int i = 0; i < FIG_SIZE; i++) free(figure[i]);
        free(figure);
        figure = NULL;
    }
}

void update_figures(figure *fig, figure *next_fig) {
    fig->type = next_fig->type;
    fig->spawn_x = next_fig->spawn_x;
    fig->spawn_y = next_fig->spawn_y;

    for (int i = 0; i < FIG_SIZE; i++)
        for (int j = 0; j < FIG_SIZE; j++)
            fig->figure[i][j] = next_fig->figure[i][j];

    remove_figure(next_fig->figure);
    init_new_figure(next_fig);
}

void fill_figure_matrix(figure *fig) {
    switch (fig->type) {
        case Stick:
            for (int j = 0; j < FIG_SIZE; j++) fig->figure[1][j] = 1;
            break;
        case Left_L:
            fig->figure[0][0] = 1;
            for (int j = 0; j < FIG_SIZE - 1; j++) fig->figure[1][j] = 1;
            break;
        case Right_L:
            fig->figure[0][2] = 1;
            for (int j = 0; j < FIG_SIZE - 1; j++) fig->figure[1][j] = 1;
            break;
        case Cube:
            fig->figure[1][1] = fig->figure[1][2] = fig->figure[2][1] = fig->figure[2][2] = 1;
            break;
        case Right_Z:
            fig->figure[1][1] = fig->figure[1][2] = fig->figure[2][0] = fig->figure[2][1] = 1;
            break;
        case Fuck:
            fig->figure[1][1] = 1;
            for (int j = 0; j < FIG_SIZE - 1; j++) fig->figure[2][j] = 1;
            break;
        case Left_Z:
            fig->figure[1][0] = fig->figure[1][1] = fig->figure[2][1] = fig->figure[2][2] = 1;
            break;
    }
}

block get_block_location(figure fig, int block_number) {
    block location = {};
    int count = 0, empty_line = 1;
    for (int i = 0; i < FIG_SIZE; i++)
        for (int j = 0; j < FIG_SIZE; j++) {
            if (!fig.figure[i][j]) continue;
            if (count == block_number) {
                location.x = j;
                location.y = i;
            }
            count++;
        }

    count = 0;
    for (int i = 0; i < FIG_SIZE; i++) {
        for (int j = 0; j < FIG_SIZE; j++) {
            if (fig.figure[i][j]) empty_line = 0;
        }
        if (!empty_line) break;
        count++;
    }
    location.y -= count;
    return location;
}

void spawn_figure(figure fig, int **field) {
    block loc = {};
    for (int i = 0; i < BLOCK_NUMBER; i++) {
        loc = get_block_location(fig, i);
        field[fig.spawn_y + loc.y][fig.spawn_x + loc.x] = fig.type;
        loc.x = 0, loc.y = 0;
    }
}

void despawn_figure(figure fig, int **field) {
    block loc = {};
    for (int i = 0; i < BLOCK_NUMBER; i++) {
        loc = get_block_location(fig, i);
        field[fig.spawn_y + loc.y][fig.spawn_x + loc.x] = Empty;
        loc.x = 0, loc.y = 0;
    }
}

void rotate_figure(figure *fig, int **field) {
    despawn_figure(*fig, field);
    for (int i = 0; i < FIG_SIZE; i++) {
        for (int j = i + 1; j < FIG_SIZE; j++) {
            int temp = fig->figure[i][j];
            fig->figure[i][j] = fig->figure[j][i];
            fig->figure[j][i] = temp;
        }
    }
    for (int i = 0; i < FIG_SIZE; i++) {
        for (int j = 0; j < FIG_SIZE / 2; j++) {
            int temp = fig->figure[i][j];
            fig->figure[i][j] = fig->figure[i][FIG_SIZE - 1 - j];
            fig->figure[i][FIG_SIZE - 1 - j] = temp;
        }
    }
    if (collision(*fig, field)) {
        rotate_figure_back(fig, field);
    } else {
        spawn_figure(*fig, field);
    }
}

void figure_move_left(figure *fig, int **field) {
    despawn_figure(*fig, field);
    fig->spawn_x--;
    if (collision(*fig, field)) fig->spawn_x++;

    spawn_figure(*fig, field);
}

void figure_move_right(figure *fig, int **field) {
    despawn_figure(*fig, field);
    fig->spawn_x++;
    if (collision(*fig, field)) fig->spawn_x--;

    spawn_figure(*fig, field);
}

void figure_move_down(figure *fig, figure *next_fig, int **field) {
    despawn_figure(*fig, field);
    fig->spawn_y++;
    if (collision(*fig, field)) {
        fig->spawn_y--;
        spawn_figure(*fig, field);
        update_figures(fig, next_fig);
    }
    spawn_figure(*fig, field);
}

int collision(figure fig, int **field) {
    int collision = 0;
    block loc;
    for (int i = 0; i < BLOCK_NUMBER; i++) {
        loc = get_block_location(fig, i);

        if (fig.spawn_x + loc.x > WIDTH - 1 || fig.spawn_x + loc.x < 0 ||
            fig.spawn_y + loc.y > HEIGHT - 1) {
            collision = 1;
            break;
        }
        if (field[fig.spawn_y + loc.y][fig.spawn_x + loc.x] != Empty) {
            collision = 1;
            break;
        }
    }
    return collision;
}

int line_is_filled(int **field, int row_number) {
    int filled = 1;
    for (int j = 0; j < WIDTH; j++)
        if (field[row_number][j] == Empty) filled = 0;
    return filled;
}

int line_is_empty(int **field, int row_number) {
    int empty = 1;
    for (int j = 0; j < WIDTH; j++)
        if (field[row_number][j] != Empty) empty = 0;
    return empty;
}

void delete_row(int **field, int row_number) {
    for (int i = row_number; i > 0; i--) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = field[i - 1][j];
        }
    }
}

int remove_filled_rows(int **field) {
    int calc = 0;
    figure *fig = get_current_figure();
    despawn_figure(*fig, field);
    for (int i = 0; i < HEIGHT; i++) {
        if (line_is_filled(field, i)) {
            delete_row(field, i);
            calc++;
        }
    }
    spawn_figure(*fig, field);
    return calc;
}

int calc_score(game_info *gi, int filled_lines) {
    switch (filled_lines) {
        case 0:
            break;
        case 1:
            gi->score += 100;
            break;
        case 2:
            gi->score += 300;
            break;
        case 3:
            gi->score += 700;
            break;
        case 4:
            gi->score += 1500;
            break;
    }
    if (gi->score > gi->high_score) gi->high_score = gi->score;
    return gi->score;
}

int is_game_over(game_info *gi, figure *fig, figure *next_fig) {
    int game_over = 0;

    if (!line_is_empty(gi->next, 0)) {
        despawn_figure(*fig, gi->next);
        fig->spawn_y++;
        if (collision(*fig, gi->next) || collision(*next_fig, gi->next))
            game_over = 1;
        fig->spawn_y--;
        spawn_figure(*fig, gi->next);
    }

    if (!game_over) spawn_figure(*fig, gi->next);
    return game_over;
}

game_info update_current_state() {
    int *tick = get_tick();
    game_info *gi = get_game_info_instance();
    figure *fig = get_current_figure();
    figure *next_fig = get_next_figure();

    copy_field(gi->next, gi->field);

    if (gi->pause == 0 && (*tick) >= gi->speed) {
        figure_move_down(fig, next_fig, gi->next);
        *tick = 0;
    } else if (gi->pause == 0) {
        (*tick)++;
    }

    gi->score = calc_score(gi, remove_filled_rows(gi->next));
    if (gi->score / 600 >= gi->level && gi->level < 10) {
        gi->level++;
        gi->speed -= SPEED_STEP;
    }
    write_high_score(gi);
    if (is_game_over(gi, fig, next_fig)) {
        gi->pause = 2;
    }
    return *gi;
}

void user_input(UserAction_t action, bool hold) {
    game_info *gi = get_game_info_instance();
    figure *fig = get_current_figure();
    int *tick = get_tick();
    if (hold) {
    };
    switch ((gi->pause == 1 && action != Start && action != Terminate) ? Pause
                                                                       : action) {
        case Start:
            gi->pause = 0;
            break;
        case Pause:
            gi->pause = 1;
            break;
        case Terminate:
            gi->pause = 3;
            break;
        case Left:
            figure_move_left(fig, gi->next);
            break;
        case Right:
            figure_move_right(fig, gi->next);
            break;
        case Up:
            rotate_figure(fig, gi->next);
            break;
        case Down:
            (*tick) += gi->speed / 2;
            break;
        case Action:
            rotate_figure(fig, gi->next);
            break;
    }
}

void write_high_score(game_info *gi) {
    FILE *file_score = fopen("high_score.txt", "w");
    fprintf(file_score, "%d", gi->high_score);
    fclose(file_score);
}

void read_high_score(game_info *gi) {
    FILE *file_score = fopen("high_score.txt", "r");
    if (file_score == NULL) {
        gi->high_score = 0;
    } else {
        fscanf(file_score, "%d", &gi->high_score);
        fclose(file_score);
    }
}

void rotate_figure_back(figure *fig, int **field) {
    for (int i = 0; i < FIG_SIZE; i++) {
        for (int j = 0; j < FIG_SIZE / 2; j++) {
            int temp = fig->figure[i][j];
            fig->figure[i][j] = fig->figure[i][FIG_SIZE - 1 - j];
            fig->figure[i][FIG_SIZE - 1 - j] = temp;
        }
    }
    for (int i = 0; i < FIG_SIZE; i++) {
        for (int j = i + 1; j < FIG_SIZE; j++) {
            int temp = fig->figure[i][j];
            fig->figure[i][j] = fig->figure[j][i];
            fig->figure[j][i] = temp;
        }
    }
    spawn_figure(*fig, field);
}

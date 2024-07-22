#include "test.h"

int main() {
    int failed = 0;
    int all = 0;
    int success = 0;
    Suite *list_cases[] = {test_brick_game_tetris(),
                           NULL};

    for (int i = 0; list_cases[i] != NULL; i++) {
        SRunner *sr = srunner_create(list_cases[i]);
        srunner_set_fork_status(sr, CK_NOFORK);
        srunner_run_all(sr, CK_NORMAL);
        all += srunner_ntests_run(sr);
        failed += srunner_ntests_failed(sr);
        srunner_free(sr);
    }
    success = all - failed;
    printf("\033[45m\t----------ALL: %d--------------\t\t\033[0m \n", all);
    printf("\033[45m\t----------SUCCESS: %d----------\t\t\033[0m \n", success);
    printf("\033[45m\t----------FAILED: %d------------\t\t\033[0m \n", failed);

    return failed == 0 ? 0 : 1;
}
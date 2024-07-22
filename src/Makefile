CC = gcc
FLAGS = -std=c11 -Wall -Werror -Wextra
TETRIS_PATH = brickgame/tetris/tetris.c
CLI_PATH = gui/cli/cli.c
OBJECTS = *.o
TFLAGS = -lcheck --coverage

ifeq ($(shell uname), Linux)
	TFLAGS += -lsubunit -lm
endif

ifeq ($(shell uname), Darwin)
	TFLAGS += -fprofile-arcs -ftest-coverage
endif


all: clean install

install: build
	install tetris ~/Desktop/tetris
	@rm *.o

uninstall:
	rm -rf ~/Desktop/tetris

build: $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -lncurses -o tetris

tetris.a: 
	$(CC) -c $(FLAGS) $(TETRIS_PATH)
	ar rc tetris.a tetris.o
	ranlib tetris.a

$(OBJECTS): main.c brickgame/tetris/tetris.c gui/cli/cli.c 
	$(CC) $(FLAGS) -c main.c $^

tetris_test.a: 
	$(CC) $(CFLAGS) $(TFLAGS) -c brickgame/tetris/tetris.c
	ar rcs $@ tetris.o

test: tetris_test.a
	$(CC) -o test.a brickgame/tetris/tetris_test.c tetris_test.a $(TFLAGS)
	./test.a

gcov_report: test
	gcov -b ./brickgame/tetris/tetris.c ./brickgame/tetris/tetris_test.c
	lcov -d . -c -o coverage.info
	genhtml coverage.info -o html_report
	open html_report/index.html

clean: uninstall
	rm -rf tetris tetris-1.0 tetris-1.0.tar.gz *.o tetris.log tetris.toc tetris.dvi *.gcno *.gcda *.a test.a coverage.info html_report docs
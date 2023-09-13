#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

#include "util.h"

void errorAndExit(char *msg) {
    endwin();
    printf("ERROR: %s\n", msg);
    exit(EXIT_FAILURE);
}

void errorAndExitf(char *msg, char *detail) {
    endwin();
    printf("ERROR: [%s] %s\n", detail, msg);
    exit(EXIT_FAILURE);
}

void todo(int line, char *file) {
    endwin();
    printf("TODO: %s:%d\n", file, line);
    exit(EXIT_FAILURE);
}

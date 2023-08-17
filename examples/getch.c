#include <stdio.h>
#include <curses.h>

int main() {
    initscr();
    keypad(stdscr, true);
    noecho();
    curs_set(0);

    int key = KEY_BACKSPACE;

    if (has_key(key)) {
        printw("yay");
    } else {
        printw("nay");
    }

    int c = wgetch(stdscr);

    if (c == key) {
        printw("yes");
    } else {
        printw("no");
    }

    printf("%d", KEY_BACKSPACE);

    printw("%d", c);

    getch();

    return 0;
}

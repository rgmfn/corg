#include <curses.h>

int main(int argc, char ** argv) {
    initscr();

    int height, width, start_y, start_x;
    height = 10;
    width = 20;
    start_y = start_x = 10;

    addstr("testing");

    WINDOW *win = newwin(height, width, start_y, start_x);
    refresh();

    box(win, 0, 0);
    mvwprintw(win, 1, 1, "this is my box");
    wrefresh(win);

    int c = getch();

    endwin();

    return 0;
}

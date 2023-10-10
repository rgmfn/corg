#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "node.h"
#include "colors.h"
#include "fileio.h"
#include "state.h"
#include "windows.h"
#include "input.h"
#include "util.h"

int main(int argc, char *argv[]) {
    initscr();
    noecho();
    curs_set(0);

    if (has_colors()) {
        start_color();

        for (int i = RED; i <= BRIGHT_WHITE; i++) {
            init_pair(i, i, COLOR_BLACK);
            init_pair(i+15, i, GRAY);
        }
        init_pair(GRAY_SEL, COLOR_BLACK, GRAY);
    }

    Node *head = loadFromFile(argv[1]);

    app.head = head;
    app.curr = head->next;
    app.focus = Document;
    app.topLine = head->next;
    
    calendar.curr = getToday();

    input.cursorPos = 0;
    strncpy(input.string, "", sizeof(input.string));

    app.isRunning = true;
    while (app.isRunning) {
        clear();
        drawDocument();

        if (app.popupWin != NULL) {
            wclear(app.popupWin);
            drawPopupWindow();
        }

        app.c = getch();
        parseInput();
    }

    freeSubtree(app.head);
    endwin();

    return EXIT_SUCCESS;
}

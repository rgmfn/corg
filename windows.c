#include <curses.h>

#include "windows.h"
#include "node.h"
#include "state.h"
#include "colors.h"

WINDOW* newCenteredWin(int height, int width) {
    int begin_y = LINES/2 - height/2;
    int begin_x = COLS/2 - width/2;

    return newwin(height, width, begin_y, begin_x);
}

void drawDocument() {
    move(0, 0);
    printTree(&state.head, state.curr, 0);
    refresh();
}

void drawPopupWindow() {
    switch (state.focus) {
        case Document:
            break;
        case TodoWindow:
            drawTodoWindow();
            break;
        default:
            drawTempWindow();
            break;
    }
}

void drawTodoWindow() {
    // --- ROW1 ---
    mvwprintw(state.popupWin, 1, 1, "[t] ");
    wattrset(state.popupWin, COLOR_PAIR(GREEN));
    wprintw(state.popupWin, "TODO  ");
    wattrset(state.popupWin, COLOR_PAIR(0));

    wprintw(state.popupWin, "[p] ");
    wattrset(state.popupWin, COLOR_PAIR(GRAY));
    wprintw(state.popupWin, "PROJ  ");
    wattrset(state.popupWin, COLOR_PAIR(0));

    wprintw(state.popupWin, "[r] ");
    wattrset(state.popupWin, COLOR_PAIR(GREEN));
    wprintw(state.popupWin, "LOOP");
    wattrset(state.popupWin, COLOR_PAIR(0));

    // --- ROW2 ---
    mvwprintw(state.popupWin, 2, 1, "[s] ");
    wattrset(state.popupWin, COLOR_PAIR(MAGENTA));
    wprintw(state.popupWin, "STRT  ");
    wattrset(state.popupWin, COLOR_PAIR(0));

    wprintw(state.popupWin, "[w] ");
    wattrset(state.popupWin, COLOR_PAIR(YELLOW));
    wprintw(state.popupWin, "WAIT  ");
    wattrset(state.popupWin, COLOR_PAIR(0));

    wprintw(state.popupWin, "[h] ");
    wattrset(state.popupWin, COLOR_PAIR(YELLOW));
    wprintw(state.popupWin, "HOLD");
    wattrset(state.popupWin, COLOR_PAIR(0));

    // --- ROW3 ---
    mvwprintw(state.popupWin, 3, 1, "[i] ");
    wattrset(state.popupWin, COLOR_PAIR(GREEN));
    wprintw(state.popupWin, "IDEA  ");
    wattrset(state.popupWin, COLOR_PAIR(0));

    wprintw(state.popupWin, "[d] ");
    wattrset(state.popupWin, COLOR_PAIR(GRAY));
    wprintw(state.popupWin, "DONE  ");
    wattrset(state.popupWin, COLOR_PAIR(0));

    wprintw(state.popupWin, "[k] ");
    wattrset(state.popupWin, COLOR_PAIR(RED));
    wprintw(state.popupWin, "KILL");
    wattrset(state.popupWin, COLOR_PAIR(0));

    // --- ROW4 ---
    mvwhline(state.popupWin, 4, 1, 0, COLS);

    // --- ROW5 ---
    mvwprintw(state.popupWin, 5, 1, "[T] ");
    wattrset(state.popupWin, COLOR_PAIR(GREEN));
    wprintw(state.popupWin, "[ ]   ");
    wattrset(state.popupWin, COLOR_PAIR(0));

    wprintw(state.popupWin, "[S] ");
    wattrset(state.popupWin, COLOR_PAIR(MAGENTA));
    wprintw(state.popupWin, "[-]   ");
    wattrset(state.popupWin, COLOR_PAIR(0));

    wprintw(state.popupWin, "[W] ");
    wattrset(state.popupWin, COLOR_PAIR(YELLOW));
    wprintw(state.popupWin, "[?]");
    wattrset(state.popupWin, COLOR_PAIR(0));

    // --- ROW6 ---
    mvwprintw(state.popupWin, 6, 1, "[D] ");
    wattrset(state.popupWin, COLOR_PAIR(GRAY));
    wprintw(state.popupWin, "[X]   ");
    wattrset(state.popupWin, COLOR_PAIR(0));

    // --- ROW7 ---
    mvwhline(state.popupWin, 7, 1, 0, COLS);

    // --- ROW8 ---
    mvwprintw(state.popupWin, 8, 1, "[o] ");
    wattrset(state.popupWin, COLOR_PAIR(GRAY));
    wprintw(state.popupWin, "OKAY  ");
    wattrset(state.popupWin, COLOR_PAIR(0));

    wprintw(state.popupWin, "[y] ");
    wattrset(state.popupWin, COLOR_PAIR(GRAY));
    wprintw(state.popupWin, "YES   ");
    wattrset(state.popupWin, COLOR_PAIR(0));

    wprintw(state.popupWin, "[n] ");
    wattrset(state.popupWin, COLOR_PAIR(RED));
    wprintw(state.popupWin, "NO");
    wattrset(state.popupWin, COLOR_PAIR(0));

    // --- ROW9 ---
    mvwhline(state.popupWin, 9, 1, 0, COLS);

    // --- ROW10 ---
    mvwprintw(state.popupWin, 10, 1, "[N] ");
    wprintw(state.popupWin, "NONE");
    wattrset(state.popupWin, COLOR_PAIR(0));

    box(state.popupWin, 0, 0);

    wrefresh(state.popupWin);
}

void drawTempWindow() {
    box(state.popupWin, 0, 0);
    mvwprintw(state.popupWin, 2, 1, "Temp Window");
    wrefresh(state.popupWin);
}

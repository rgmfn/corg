#include <curses.h>
#include <string.h>
#include <time.h>

#include "fileio.h"
#include "windows.h"
#include "node.h"
#include "state.h"
#include "colors.h"
#include "util.h"

WINDOW* newCenteredWin(int height, int width) {
    int begin_y = LINES/2 - height/2;
    int begin_x = COLS/2 - width/2;

    return newwin(height, width, begin_y, begin_x);
}

WINDOW* getTodoWindow() {
    return newCenteredWin(12, 30);
}

WINDOW* getCalendarWindow() {
    return newCenteredWin(CALENDAR_COLS, CALENDAR_LINES);
}

WINDOW* getInputWindow() {
    return newCenteredWin(INPUT_LINES, INPUT_COLS);
}

void drawDocument() {
    move(0, 0);
    /* printTree(app.head, 0); */
    printPartialTree(app.topLine, LINES);
    refresh();
}

void drawPopupWindow() {
    switch (app.focus) {
        case Document:
            break;
        case TodoWindow:
            drawTodoWindow();
            break;
        case RenameWindow:
            drawInputWindow("Change name");
            break;
        case DescriptionWindow:
            drawInputWindow("Change description");
            break;
        case FilenameWindow:
            drawInputWindow("Save to file");
            break;
        case CalendarWindow:
            drawCalendarWindow();
            break;
        default:
            drawTempWindow();
            break;
    }
}

void drawTodoWindow() {
    // --- ROW1 ---
    mvwprintw(app.popupWin, 1, 1, "[t] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Todo)));
    wprintw(app.popupWin, "TODO  ");
    wattrset(app.popupWin, COLOR_PAIR(0));

    wprintw(app.popupWin, "[p] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Proj)));
    wprintw(app.popupWin, "PROJ  ");
    wattrset(app.popupWin, COLOR_PAIR(0));

    wprintw(app.popupWin, "[r] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Loop)));
    wprintw(app.popupWin, "LOOP");
    wattrset(app.popupWin, COLOR_PAIR(0));

    // --- ROW2 ---
    mvwprintw(app.popupWin, 2, 1, "[s] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Strt)));
    wprintw(app.popupWin, "STRT  ");
    wattrset(app.popupWin, COLOR_PAIR(0));

    wprintw(app.popupWin, "[w] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Wait)));
    wprintw(app.popupWin, "WAIT  ");
    wattrset(app.popupWin, COLOR_PAIR(0));

    wprintw(app.popupWin, "[h] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Hold)));
    wprintw(app.popupWin, "HOLD");
    wattrset(app.popupWin, COLOR_PAIR(0));

    // --- ROW3 ---
    mvwprintw(app.popupWin, 3, 1, "[i] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Idea)));
    wprintw(app.popupWin, "IDEA  ");
    wattrset(app.popupWin, COLOR_PAIR(0));

    wprintw(app.popupWin, "[d] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Done)));
    wprintw(app.popupWin, "DONE  ");
    wattrset(app.popupWin, COLOR_PAIR(0));

    wprintw(app.popupWin, "[k] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Kill)));
    wprintw(app.popupWin, "KILL");
    wattrset(app.popupWin, COLOR_PAIR(0));

    // --- ROW4 ---
    mvwhline(app.popupWin, 4, 1, 0, COLS);

    // --- ROW5 ---
    mvwprintw(app.popupWin, 5, 1, "[T] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Unchecked)));
    wprintw(app.popupWin, "[ ]   ");
    wattrset(app.popupWin, COLOR_PAIR(0));

    wprintw(app.popupWin, "[S] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Started)));
    wprintw(app.popupWin, "[-]   ");
    wattrset(app.popupWin, COLOR_PAIR(0));

    wprintw(app.popupWin, "[W] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Waiting)));
    wprintw(app.popupWin, "[?]");
    wattrset(app.popupWin, COLOR_PAIR(0));

    // --- ROW6 ---
    mvwprintw(app.popupWin, 6, 1, "[D] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Checked)));
    wprintw(app.popupWin, "[X]   ");
    wattrset(app.popupWin, COLOR_PAIR(0));

    // --- ROW7 ---
    mvwhline(app.popupWin, 7, 1, 0, COLS);

    // --- ROW8 ---
    mvwprintw(app.popupWin, 8, 1, "[o] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Okay)));
    wprintw(app.popupWin, "OKAY  ");
    wattrset(app.popupWin, COLOR_PAIR(0));

    wprintw(app.popupWin, "[y] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(Yes)));
    wprintw(app.popupWin, "YES   ");
    wattrset(app.popupWin, COLOR_PAIR(0));

    wprintw(app.popupWin, "[n] ");
    wattrset(app.popupWin, COLOR_PAIR(getTypeColor(No)));
    wprintw(app.popupWin, "NO");
    wattrset(app.popupWin, COLOR_PAIR(0));

    // --- ROW9 ---
    mvwhline(app.popupWin, 9, 1, 0, COLS);

    // --- ROW10 ---
    mvwprintw(app.popupWin, 10, 1, "[N] ");
    wprintw(app.popupWin, "NONE");
    wattrset(app.popupWin, COLOR_PAIR(0));

    box(app.popupWin, 0, 0);

    wrefresh(app.popupWin);
}

void drawCalendarWindow() {
    // --- ROW1 ---
    time_t t = time(NULL);
    struct tm today = *localtime(&t);
    today.tm_mon = 9;
    today.tm_mday = 16;

    char month[20];
    sprintf(month, "%s", getMonthFromInt(today.tm_mon));
    int monthLen = strnlen(month, sizeof(month))+5; // + " 20XX"
    wattrset(app.popupWin, COLOR_PAIR(GREEN));
    mvwprintw(app.popupWin, 1, CALENDAR_LINES/2-(monthLen+1)/2, month);

    mvwprintw(app.popupWin, 1, CALENDAR_LINES/2+(monthLen-8)/2, "%d", (today.tm_year+1900));

    // --- ROW2 ---
    wattrset(app.popupWin, COLOR_PAIR(GRAY));
    mvwprintw(app.popupWin, 2, 2, "Su                Sa");
    wattrset(app.popupWin, COLOR_PAIR(CYAN));
    mvwprintw(app.popupWin, 2, 5, "Mo Tu We Th Fr");
    wattrset(app.popupWin, COLOR_PAIR(0));

    // --- ROW3-7 ---
    struct tm firstOfMonth = getFirstOfMonth(today);
    int offset = firstOfMonth.tm_wday;
    int daysInMonth = getDaysInMonth(today.tm_mon, today.tm_year+1900);

    for (int day = 1; day <= daysInMonth; day++) {
        int col = ((day-1 + offset)%7)*3+2;
        int line = (day-1 + offset)/7+3;

        mvwprintw(app.popupWin, line, col, "%d", day);
    }
    
    /* TODO; // show current day selected */
    /* TODO; // call function for days in a month */

    // --- ROW8 ---
    /* TODO; // print stuff about prev month, next month, today */

    box(app.popupWin, 0, 0);

    wrefresh(app.popupWin);
}

void drawInputWindow(char *name) {
    box(app.popupWin, 0, 0);

    mvwprintw(app.popupWin, 1, 1, input.string);

    wattrset(app.popupWin, COLOR_PAIR(WHITE_SEL));
    mvwaddch(app.popupWin, 1, 1+input.cursorPos, ' ');
    wattrset(app.popupWin, COLOR_PAIR(0));

    mvwaddstr(app.popupWin, 0, 1, name);

    wrefresh(app.popupWin);
}

void drawTempWindow() {
    box(app.popupWin, 0, 0);
    mvwprintw(app.popupWin, 1, 1, "Temp Window");
    wrefresh(app.popupWin);
}

void windentNTimes(WINDOW *win, int n) {
    for (int i = 0; i < n; i++) {
        waddstr(win, "  ");
    }
}

void openCalendarWindow() {
    app.popupWin = getCalendarWindow();
    refresh();
    app.focus = CalendarWindow;
}

void openTodoWindow() {
    app.popupWin = getTodoWindow();
    refresh();
    app.focus = TodoWindow;
}

void openRenameWindow() {
    app.popupWin = getInputWindow();
    refresh();
    strncpy(input.string, app.curr->name, sizeof(input.string));
    input.cursorPos = strnlen(input.string, sizeof(input.string));
    app.focus = RenameWindow;
}

void openDescriptionWindow() {
    app.popupWin = getInputWindow();
    refresh();
    strncpy(input.string, app.curr->description, sizeof(input.string));
    input.cursorPos = strnlen(input.string, sizeof(input.string));
    app.focus = DescriptionWindow;
}

void openFilenameWindow() {
    app.popupWin = getInputWindow();
    refresh();
    strncpy(input.string, app.filename, sizeof(input.string));
    input.cursorPos = strnlen(input.string, sizeof(input.string));
    app.focus = FilenameWindow;
}
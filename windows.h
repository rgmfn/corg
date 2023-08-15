#ifndef WINDOWS_H
#define WINDOWS_H

#include "curses.h"

typedef enum AppFocus {
    Document,
    TodoWindow,
    CalendarWindow,
} AppFocus;

WINDOW* newCenteredWin(int, int);

void drawDocument();
void drawPopupWindow();
void drawTodoWindow();
void drawTempWindow();

#endif

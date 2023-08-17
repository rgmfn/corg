#ifndef WINDOWS_H
#define WINDOWS_H

#include "curses.h"

typedef enum AppFocus {
    Document,
    TodoWindow,
    CalendarWindow,
    InputWindow,
} AppFocus;

WINDOW* newCenteredWin(int, int);

WINDOW* getTodoWindow();
WINDOW* getInputWindow();

void drawDocument();
void drawPopupWindow();
void drawTodoWindow();
void drawInputWindow();
void drawTempWindow();

#endif

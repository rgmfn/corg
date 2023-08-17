#ifndef WINDOWS_H
#define WINDOWS_H

#include "curses.h"

typedef enum AppFocus {
    Document,
    TodoWindow,
    CalendarWindow,
    RenameWindow,
    DescriptionWindow,
} AppFocus;

WINDOW* newCenteredWin(int, int);

WINDOW* getTodoWindow();
WINDOW* getInputWindow();

void drawDocument();
void drawPopupWindow();
void drawTodoWindow();
void drawInputWindow(char*);
void drawTempWindow();

void indentNTimes(WINDOW*, int);

#endif

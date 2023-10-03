#ifndef WINDOWS_H
#define WINDOWS_H

#include "curses.h"

#define INPUT_COLS 53
#define INPUT_LINES 3

#define CALENDAR_COLS 24
#define CALENDAR_LINES 12

typedef enum AppFocus {
    Document,
    TodoWindow,
    CalendarWindow,
    RenameWindow,
    DescriptionWindow,
    FilenameWindow,
} AppFocus;

WINDOW* newCenteredWin(int, int);

WINDOW* getTodoWindow();
WINDOW* getCalendarWindow();
WINDOW* getInputWindow();

void drawDocument();
void drawPopupWindow();
void drawTodoWindow();
void drawCalendarWindow();
void drawInputWindow(char*);
void drawTempWindow();

void windentNTimes(WINDOW*, int);

void openTodoWindow();
void openCalendarWindow();
void openRenameWindow();
void openDescriptionWindow();
void openFilenameWindow();

#endif

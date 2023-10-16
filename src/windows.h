#ifndef WINDOWS_H
#define WINDOWS_H

#include "curses.h"

#define INPUT_COLS 53
#define INPUT_LINES 3

#define CALENDAR_COLS 24
#define CALENDAR_LINES 11

typedef enum AppFocus {
    Document,
    TodoWindow,
    CalendarWindow,
    RenameWindow,
    DescriptionWindow,
    FilenameWindow,
    ErrorWindow,
} AppFocus;

WINDOW* newCenteredWin(int, int);

WINDOW* getTodoWindow();
WINDOW* getCalendarWindow();
WINDOW* getInputWindow();
WINDOW* getErrorWindow();

void drawDocument();
void drawPopupWindow();
void drawTodoWindow();
void drawCalendarWindow();
void drawErrorWindow();
void drawInputWindow(char*);
void drawTempWindow();

void windentNTimes(WINDOW*, int);

void openTodoWindow();
void openCalendarWindow();
void openRenameWindow();
void openDescriptionWindow();
void openFilenameWindow();
void openErrorWindow();

void closePopupWindow();

#endif

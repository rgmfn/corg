#ifndef WINDOWS_H
#define WINDOWS_H

#include "curses.h"
#include "util.h"

#define INPUT_COLS 53
#define INPUT_LINES 3

#define ERROR_COLS 53
#define ERROR_LINES 5

#define CALENDAR_COLS 24
#define CALENDAR_LINES 11

typedef enum AppFocus {
    Document,
    TodoWindow,
    TimestampWindow,
    DeadlineWindow,
    ScheduledWindow,
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
void openCalendarWindow(AppFocus);
void openRenameWindow();
void openDescriptionWindow();
void openFilenameWindow();
void openErrorWindow();

void closePopupWindow();

#endif

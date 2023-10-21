#ifndef WINDOWS_H
#define WINDOWS_H

#include "curses.h"
#include "util.h"

#define TODO_LINES 12
#define TODO_COLS 30

#define DATETYPE_LINES 16
#define DATETYPE_COLS 31

#define INPUT_LINES 3
#define INPUT_COLS 53

#define ERROR_LINES 3
#define ERROR_COLS 53

#define CALENDAR_LINES 11
#define CALENDAR_COLS 24

typedef enum AppFocus {
    Document,
    TodoWindow,
    CalendarWindow,
    DateTypeWindow,
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
WINDOW* getDateTypeWindow();
WINDOW* getCalendarWindow();
WINDOW* getInputWindow();
WINDOW* getErrorWindow();

void drawDocument();
void drawPopupWindow();
void drawTodoWindow();
void drawDateTypeWindow();
void drawCalendarWindow();
void drawErrorWindow();
void drawInputWindow(char*);
void drawTempWindow();

void windentNTimes(WINDOW*, int);

void openTodoWindow();
void openCalendarWindow(DateType);
void openDateTypeWindow();
void openRenameWindow();
void openDescriptionWindow();
void openFilenameWindow();
void openErrorWindow(const char*);

void closePopupWindow();

#endif

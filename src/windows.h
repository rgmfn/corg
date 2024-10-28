/*
 * Copyright (C) 2023 Ryan Gragg.
 *
 * This file is part of Corg.
 *
 * Corg is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Corg is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Corg. If not, see <https://www.gnu.org/licenses/>. 
 */

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

#define HELP_LINES 15
#define HELP_COLS 40

#define ERROR_LINES 3
#define ERROR_COLS 53

#define CALENDAR_LINES 12
#define CALENDAR_COLS 24

#define PRIORITY_LINES 7
#define PRIORITY_COLS 31

typedef enum AppFocus {
    Document,
    TodoWindow,
    CalendarWindow,
    DateTypeWindow,
    RenameWindow,
    DescriptionWindow,
    LinkWindow,
    FilenameWindow,
    PriorityWindow,
    HelpWindow,
    ErrorWindow,
} AppFocus;

WINDOW* newCenteredWin(int, int);

WINDOW* getTodoWindow();
WINDOW* getDateTypeWindow();
WINDOW* getCalendarWindow();
WINDOW* getInputWindow();
WINDOW* getHelpWindow();
WINDOW* getErrorWindow();
WINDOW* getPriorityWindow();

void drawDocument();
void drawPopupWindow();
void drawTodoWindow();
void drawDateTypeWindow();
void drawCalendarWindow();
void drawHelpWindow();
void drawErrorWindow();
void drawPriorityWindow();
void drawInputWindow(char*);
void drawTempWindow();

void windentNTimes(WINDOW*, int);

void openTodoWindow();
void openCalendarWindow(DateType);
void openDateTypeWindow();
void openRenameWindow();
void openDescriptionWindow();
void openLinkWindow();
void openFilenameWindow();
void openPriorityWindow();
void openHelpWindow();
void openErrorWindow(const char*);

void closePopupWindow();

#endif

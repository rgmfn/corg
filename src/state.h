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

#ifndef STATE_H
#define STATE_H

#include <curses.h>
#include <time.h>

#include "node.h"
#include "util.h"
#include "windows.h"

#define INPUT_SIZE 50

struct AppState {
    Node *head;
    Node *curr;
    Node *topLine;
    AppFocus focus;
    WINDOW *popupWin;

    char filename[50];
    char errbuf[100];

    int c;

    bool isRunning;
} app;

struct InputState {
    char string[INPUT_SIZE];
    int cursorPos;
} input;

struct CalendarState {
     struct tm curr;
     DateType dateType;
     // haven't decided if 'curr' should mean current or cursor
} calendar;

#endif

#ifndef STATE_H
#define STATE_H

#include <curses.h>
#include <time.h>

#include "node.h"
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
     // haven't decided if 'curr' should mean current or cursor
} calendar;

#endif

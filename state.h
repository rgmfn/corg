#ifndef STATE_H
#define STATE_H

#include <curses.h>

#include "node.h"
#include "windows.h"

struct AppState {
    Node *head;
    Node *curr;
    Node *topLine;
    AppFocus focus;
    WINDOW *popupWin;

    char filename[50];

    int c;

    bool isRunning;
} app;

struct InputState {
    char string[50];
    int cursorPos;
} input;

#endif

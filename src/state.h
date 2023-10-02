#ifndef STATE_H
#define STATE_H

#include <curses.h>

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

    int c;

    bool isRunning;
} app;

struct InputState {
    char string[INPUT_SIZE];
    int cursorPos;
} input;

#endif

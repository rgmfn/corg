#ifndef STATE_H
#define STATE_H

#include <curses.h>

#include "node.h"
#include "windows.h"

struct ProgramState {
    Node head;
    Node *curr;
    AppFocus focus;
    WINDOW *popupWin;

    char c;

    bool appIsRunning;
} state;

#endif

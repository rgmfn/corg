#include "input.h"
#include "state.h"

void parseInput() {
    addstr("parseInput");
    switch (state.focus) {
        case Document:
            parseDocumentInput();
            break;
        case TodoWindow:
            parseTodoInput();
            break;
        default:
            break;
    }
}

void parseDocumentInput() {
    addstr("parseDocumentInput");
    switch (state.c) {
        case 'j':
            state.curr = goDownVisual(state.curr);
            break;
        case 'k':
            state.curr = goUpVisual(state.curr);
            break;
        case 'h':
            state.curr = goPrevLogical(state.curr);
            break;
        case 'H':
            state.curr = gotoParent(state.curr);
            break;
        case 'l':
            state.curr = goNextLogical(state.curr);
            break;
        case '\t':
            toggleSubtree(state.curr);
            break;
        case 't':
            state.popupWin = newCenteredWin(12, 30);
            refresh();
            state.focus = TodoWindow;
            break;
        case 'q':
            state.appIsRunning = false;
            break;
        default:
            break;
    }
}

void parseTodoInput() {
    switch (state.c) {
        case 't':
            state.curr->type = Todo;
            break;
        case 'p':
            state.curr->type = Proj;
            break;
        case 'r':
            state.curr->type = Loop;
            break;
        case 's':
            state.curr->type = Strt;
            break;
        case 'w':
            state.curr->type = Wait;
            break;
        case 'h':
            state.curr->type = Hold;
            break;
        case 'i':
            state.curr->type = Idea;
            break;
        case 'd':
            state.curr->type = Done;
            break;
        case 'k':
            state.curr->type = Kill;
            break;
        case 'T':
            state.curr->type = Unchecked;
            break;
        case 'S':
            state.curr->type = Started;
            break;
        case 'W':
            state.curr->type = Waiting;
            break;
        case 'D':
            state.curr->type = Checked;
            break;
        case 'o':
            state.curr->type = Okay;
            break;
        case 'y':
            state.curr->type = Yes;
            break;
        case 'n':
            state.curr->type = No;
            break;
        case 'N':
            state.curr->type = None;
            break;
        default:
            break;
    }

    state.focus = Document;
    state.popupWin = NULL;
}

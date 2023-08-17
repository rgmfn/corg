#include <string.h>

#include "input.h"
#include "node.h"
#include "state.h"
#include "windows.h"

void parseInput() {
    switch (app.focus) {
        case Document:
            parseDocumentInput();
            break;
        case TodoWindow:
            parseTodoInput();
            break;
        case RenameWindow:
            parseInputInput(app.curr->name);
            break;
        case DescriptionWindow:
            parseInputInput(app.curr->description);
            break;
        default:
            app.focus = Document;
            break;
    }
}

void parseDocumentInput() {
    switch (app.c) {
        case 'j':
            app.curr = goDownVisual(app.curr);
            break;
        case 'k':
            app.curr = goUpVisual(app.curr);
            break;
        case 'h':
            app.curr = goPrevLogical(app.curr);
            break;
        case 'H':
            app.curr = gotoParent(app.curr);
            break;
        case 'l':
            app.curr = goNextLogical(app.curr);
            break;
        case '\t':
            toggleSubtree(app.curr);
            break;
        case 't':
            app.popupWin = getTodoWindow();
            refresh();
            app.focus = TodoWindow;
            break;
        case 'r':
            app.popupWin = getInputWindow();
            refresh();
            strncpy(input.string, app.curr->name, sizeof(input.string));
            input.cursorPos = strnlen(input.string, sizeof(input.string));
            app.focus = RenameWindow;
            break;
        case 'd':
            app.popupWin = getInputWindow();
            refresh();
            strncpy(input.string, app.curr->description, sizeof(input.string));
            input.cursorPos = strnlen(input.string, sizeof(input.string));
            app.focus = DescriptionWindow;
            break;
        case ENTER:
            app.curr->type = cycleNodeType(app.curr->type);
            break;
        case 'q':
            app.isRunning = false;
            break;
        default:
            break;
    }
}

void parseTodoInput() {
    switch (app.c) {
        case 't':
            app.curr->type = Todo;
            break;
        case 'p':
            app.curr->type = Proj;
            break;
        case 'r':
            app.curr->type = Loop;
            break;
        case 's':
            app.curr->type = Strt;
            break;
        case 'w':
            app.curr->type = Wait;
            break;
        case 'h':
            app.curr->type = Hold;
            break;
        case 'i':
            app.curr->type = Idea;
            break;
        case 'd':
            app.curr->type = Done;
            break;
        case 'k':
            app.curr->type = Kill;
            break;
        case 'T':
            app.curr->type = Unchecked;
            break;
        case 'S':
            app.curr->type = Started;
            break;
        case 'W':
            app.curr->type = Waiting;
            break;
        case 'D':
            app.curr->type = Checked;
            break;
        case 'o':
            app.curr->type = Okay;
            break;
        case 'y':
            app.curr->type = Yes;
            break;
        case 'n':
            app.curr->type = No;
            break;
        case 'N':
            app.curr->type = None;
            break;
        default:
            break;
    }

    app.focus = Document;
    app.popupWin = NULL;
}

void parseInputInput(char *toReplace) {
    switch (app.c) {
        case ESCAPE:
            app.focus = Document;
            break;
        case ENTER:
            app.focus = Document;
            strncpy(toReplace, input.string, sizeof(app.curr->name));
            break;
        case BACKSPACE:
            input.cursorPos--;
            input.string[input.cursorPos] = '\0';
            break;
        default:
            // TODO bound check
            input.string[input.cursorPos] = app.c;
            input.cursorPos++;
            input.string[input.cursorPos] = '\0';
            break;
    } 
}

#include <string.h>

#include "fileio.h"
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
        case FilenameWindow:
            parseInputInput(app.filename);
            if (app.c == ENTER) {
                writeToFile(app.head->next, app.filename);
            }
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
            tryScrollDown(app.curr);
            break;
        case 'k':
            app.curr = goUpVisual(app.curr);
            tryScrollUp(app.curr);
            break;
        case 'h':
        case 'K':
            app.curr = goPrevLogical(app.curr);
            tryScrollUp(app.curr);
            break;
        case 'p':
            app.curr = gotoParent(app.curr);
            tryScrollUp(app.curr);
            break;
        case 'l':
        case 'J':
            app.curr = goNextLogical(app.curr);
            tryScrollDown(app.curr);
            break;
        case '\t':
            toggleSubtree(app.curr);
            break;
        case 't':
            openTodoWindow();
            break;
        case 'r':
        case 'A':
            openRenameWindow();
            break;
        case 's':
            openDescriptionWindow();
            break;
        case ENTER:
            app.curr->type = cycleNodeType(app.curr->type);
            break;
        case 'c':
            createChildNode(app.curr);
            break;
        case 'C':
            createSiblingNodeAfter(app.curr);
            break;
        case 'd':
            deleteNode(app.curr);
            break;
        case '>':
            tryPopNodeOut(app.curr);
            break;
        case '<':
            tryPopNodeIn(app.curr);
            break;
        /* case 'w': */
        /*     if (strnlen(app.filename, sizeof(app.filename)) > 0) { */
        /*         writeToFile(app.head->next, app.filename); */
        /*     } else { */
        /*         openFilenameWindow(); */
        /*     } */
        /*     break; */
        case 'a':
            openFilenameWindow();
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

/**
 * Parses the input of the InputWindow.
 *
 * @param toReplace the string to be replaced with the string created in the
 *        InputWindow.
 */
void parseInputInput(char *toReplace) {
    switch (app.c) {
        case ESCAPE:
            app.focus = Document;
            break;
        case ENTER:
            app.focus = Document;
            strncpy(toReplace, input.string, sizeof(app.curr->name));
            // TODO get change sizeof arg
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

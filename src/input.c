#include <string.h>

#include "fileio.h"
#include "input.h"
#include "node.h"
#include "state.h"
#include "windows.h"
#include "util.h"

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
        case CalendarWindow:
            parseCalendarInput();
            break;
        default:
            closePopupWindow();
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
            app.curr = goPrevLogical(app.curr);
            tryScrollUp(app.curr);
            break;
        case 'p':
            app.curr = gotoParent(app.curr);
            tryScrollUp(app.curr);
            break;
        case 'l':
            app.curr = goNextLogical(app.curr);
            tryScrollDown(app.curr);
            break;
        case 'J':
            swapNodeAndNext(app.curr);
            break;
        case 'K':
            swapNodeAndPrev(app.curr);
            break;
        case '\t':
            toggleSubtree(app.curr);
            break;
        case 't':
            openTodoWindow();
            break;
        case 'T':
            openCalendarWindow();
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
        case '0':
            // debug
            break;
        /* case 'a': */
        /*     if (strnlen(app.filename, sizeof(app.filename)) > 0) { */
        /*         writeToFile(app.head->next, app.filename); */
        /*     } else { */
        /*         openFilenameWindow(); */
        /*     } */
        /*     break; */
        case 'w':
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

    closePopupWindow();
}

void parseCalendarInput() {
    int daysInCurrMonth = getDaysInMonth(calendar.curr.tm_mon, calendar.curr.tm_year);

    switch (app.c) {
        case ESCAPE:
        case 'q':
            closePopupWindow();
            break;
        case 'j':
            calendar.curr.tm_mday += 7;
            break;
        case 'k':
        case KEY_UP:
            calendar.curr.tm_mday -= 7;
            break;
        case 'h':
            calendar.curr.tm_mday--;
            calendar.curr.tm_wday--;
            break;
        case 'l':
            calendar.curr.tm_mday++;
            calendar.curr.tm_wday++;
            break;
        case '<':
        case 'H':
        case 'J':
            calendar.curr.tm_mday -= daysInCurrMonth;
            calendar.curr.tm_wday -= daysInCurrMonth;
            break;
        case '>':
        case 'L':
        case 'K':
            calendar.curr.tm_mday += daysInCurrMonth;
            calendar.curr.tm_wday += daysInCurrMonth;
            break;
        case 'd':
            app.curr->date = NULL;
            closePopupWindow();
            break;
        case 't':
            calendar.curr = getToday();
            break;
        case ENTER:
            app.curr->date = &calendar.curr;
            closePopupWindow();
            break;
    }

    if (calendar.curr.tm_mday > daysInCurrMonth) {
        calendar.curr.tm_mday -= daysInCurrMonth;
        calendar.curr.tm_mon++;
    } else if (calendar.curr.tm_mday < 1) {
        calendar.curr.tm_mday += getDaysInMonth(calendar.curr.tm_mon-1, calendar.curr.tm_year);
        calendar.curr.tm_mon--;
    }

    if (calendar.curr.tm_mon > DECEMBER) {
        calendar.curr.tm_year++;
        calendar.curr.tm_mon = JANUARY;
    } else if (calendar.curr.tm_mon < JANUARY) {
        calendar.curr.tm_year--;
        calendar.curr.tm_mon = DECEMBER;
    }

    calendar.curr.tm_wday %= 7;
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
            closePopupWindow();
            break;
        case ENTER:
            closePopupWindow();
            strncpy(toReplace, input.string, sizeof(app.curr->name));
            // TODO get change sizeof arg
            break;
        case BACKSPACE:
            if (strnlen(input.string, INPUT_SIZE) > 0) {
                input.cursorPos--;
                input.string[input.cursorPos] = '\0';
            }
            break;
        default:
            if (strnlen(input.string, INPUT_SIZE) < 50) {
                input.string[input.cursorPos] = app.c;
                input.cursorPos++;
                input.string[input.cursorPos] = '\0';
            }
            break;
    } 
}

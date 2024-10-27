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

#include <stdlib.h>
#include <string.h>

#include "fileio.h"
#include "help.h"
#include "input.h"
#include "node.h"
#include "state.h"
#include "util.h"
#include "windows.h"

void parseInput() {
  switch (app.focus) {
  case Document:
    parseDocumentInput();
    break;
  case HelpWindow:
    if (app.c == '{')
      goPrevHelpPage();
    else if (app.c == '}')
      goNextHelpPage();
    else {
      closePopupWindow();
      parseDocumentInput();
    }
    break;
  case TodoWindow:
    parseTodoInput();
    break;
  case RenameWindow:
    parseInputInput(app.curr->name, sizeof(app.curr->name));
    break;
  case DescriptionWindow:
    parseInputInput(app.curr->description, sizeof(app.curr->description));
    break;
  case LinkWindow:
    parseInputInput(app.curr->link, sizeof(app.curr->link));
    break;
  case FilenameWindow:
    parseInputInput(app.filename, sizeof(app.filename));
    if (app.c != ENTER)
      break;
    else if (strnlen(app.filename, sizeof(app.filename)) > 0)
      writeToFile(app.head->next, app.filename);
    else
      openErrorWindow("Cannot write to a blank file.");
    break;
  case DateTypeWindow:
    parseDateTypeInput();
    break;
  case CalendarWindow:
    parseCalendarInput();
    break;
  case ErrorWindow:
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
  case 'l':
    app.curr = goNextLogical(app.curr);
    tryScrollDown(app.curr);
    break;
  case 'p':
    app.curr = gotoParent(app.curr);
    tryScrollUp(app.curr);
    break;
  case 'P':
    break;
  case 'J':
    moveNodeDown(app.curr);
    break;
  case 'K':
    moveNodeUp(app.curr);
    break;
  case '\t':
    toggleSubtree(app.curr);
    break;
  case 't':
    openTodoWindow();
    break;
  case 'm':
    openDateTypeWindow();
    break;
  case 'r':
  case 'A':
    openRenameWindow();
    break;
  case 's':
    openDescriptionWindow();
    break;
  case 'n':
    openLinkWindow();
    break;
  case 'N':
    openLink(app.curr->link);
    break;
  case ENTER:
    app.curr->type = cycleNodeType(app.curr->type);
    break;
  case 'c':
    createChildNode(app.curr);
    openRenameWindow();
    break;
  case 'C':
    createSiblingNodeAfter(app.curr);
    openRenameWindow();
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
  case '#':
    toggleCounter(app.curr);
    break;
  case '?':
    openHelpWindow();
    break;
    // only used for help page
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
  NodeType newType = app.curr->type;

  switch (app.c) {
  case 't':
    newType = Todo;
    break;
  case 'p':
    newType = Proj;
    break;
  case 'r':
    newType = Loop;
    break;
  case 's':
    newType = Strt;
    break;
  case 'w':
    newType = Wait;
    break;
  case 'h':
    newType = Hold;
    break;
  case 'i':
    newType = Idea;
    break;
  case 'd':
    newType = Done;
    break;
  case 'k':
    newType = Kill;
    break;
  case 'T':
    newType = Unchecked;
    break;
  case 'S':
    newType = Started;
    break;
  case 'W':
    newType = Waiting;
    break;
  case 'D':
    newType = Checked;
    break;
  case 'o':
    newType = Okay;
    break;
  case 'y':
    newType = Yes;
    break;
  case 'n':
    newType = No;
    break;
  case 'N':
    newType = None;
    break;
  default:
    break;
  }

  int numAnyTodoChildren = countAnyTodo(app.curr->child);
  int numDoneTodoChildren = countDoneTodo(app.curr->child);
  bool allChildrenAreFinished = numDoneTodoChildren == numAnyTodoChildren;

  if (allChildrenAreFinished || !isFinishedType(newType)) {
    app.curr->type = newType;
    closePopupWindow();
  } else {
    openErrorWindow("Not all children are in a completed state.");
  }
}

void parseDateTypeInput() {
  switch (app.c) {
  case 't':
    openCalendarWindow(Timestamp);
    break;
  case 'd':
    openCalendarWindow(Deadline);
    break;
  case 's':
    openCalendarWindow(Scheduled);
    break;
  case 'T':
    openCalendarWindow(Inactive);
    break;
  case 'c':
    openCalendarWindow(Closed);
    break;
  default:
    closePopupWindow();
    break;
  }
}

void parseCalendarInput() {
  int daysInCurrMonth =
      getDaysInMonth(calendar.curr.tm_mon, calendar.curr.tm_year);

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
    app.curr->date = malloc(sizeof(struct tm));
    app.curr->date->tm_year = calendar.curr.tm_year;
    app.curr->date->tm_mon = calendar.curr.tm_mon;
    app.curr->date->tm_mday = calendar.curr.tm_mday;
    app.curr->date->tm_wday = calendar.curr.tm_wday;
    app.curr->dateType = calendar.dateType;
    closePopupWindow();
    break;
  }

  if (calendar.curr.tm_mday > daysInCurrMonth) {
    calendar.curr.tm_mday -= daysInCurrMonth;
    calendar.curr.tm_mon++;
  } else if (calendar.curr.tm_mday < 1) {
    calendar.curr.tm_mday +=
        getDaysInMonth(calendar.curr.tm_mon - 1, calendar.curr.tm_year);
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
  // vvv needed for when wday is negative
  calendar.curr.tm_wday += 7;
  calendar.curr.tm_wday %= 7;
}

/**
 * Parses the input of the InputWindow.
 *
 * @param toReplace the string to be replaced with the string created in the
 *        InputWindow.
 */
void parseInputInput(char *toReplace, unsigned long toReplaceSize) {
  switch (app.c) {
  case ESCAPE:
    closePopupWindow();
    break;
  case ENTER:
    closePopupWindow();
    strncpy(toReplace, input.string, toReplaceSize);
    /*
     * TODO;
     * this looks like it would cause issues if
     * sizeof(input.string) > toReplaceSize, but this is never
     * the case right now
     */
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

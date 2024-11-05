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

#include <curses.h>

#include "colors.h"
#include "help.h"
#include "state.h"
#include "windows.h"

void goNextHelpPage() {
    help.page++;

    if (help.page > NUM_HELP_PAGES) {
        help.page = NUM_HELP_PAGES;
    }
}

void goPrevHelpPage() {
    help.page--;

    if (help.page < 1) {
        help.page = 1;
    }
}

void drawHelpPageOne() {
    mvwprintw(app.popupWin, 3, 1, "(q) quit");
    mvwprintw(app.popupWin, 4, 1, "(ESC) closePopup");
    mvwprintw(app.popupWin, 5, 1, "(j) goDownVisual");
    mvwprintw(app.popupWin, 6, 1, "(k) goUpVisual");
    mvwprintw(app.popupWin, 7, 1, "(h) goPrevLogical");
    mvwprintw(app.popupWin, 8, 1, "(l) goNextLogical");
    mvwprintw(app.popupWin, 9, 1, "(p) gotoParent");
    mvwprintw(app.popupWin, 10, 1, "(J) moveNodeDown");
    mvwprintw(app.popupWin, 11, 1, "(K) moveNodeUp");
}

void drawHelpPageTwo() {
    mvwprintw(app.popupWin, 3, 1, "(TAB) cycleSubtree");
    mvwprintw(app.popupWin, 4, 1, "(#) toggleCounter");
    mvwprintw(app.popupWin, 5, 1, "(ENTER) cycleNodeType");
    mvwprintw(app.popupWin, 6, 1, "(c) createChildNode");
    mvwprintw(app.popupWin, 7, 1, "(C) createSiblingNodeAfter");
    mvwprintw(app.popupWin, 8, 1, "(d) deleteNodeAndChildren");
    mvwprintw(app.popupWin, 9, 1, "(>) popNodeIn");
    mvwprintw(app.popupWin, 10, 1, "(<) popNodeOut");
}

void drawHelpPageThree() {
    mvwprintw(app.popupWin, 3, 1, "(t) openTodoWindow");
    mvwprintw(app.popupWin, 4, 1, "(m) openDateWindow");
    mvwprintw(app.popupWin, 5, 1, "(r/A) openRenameWindow");
    mvwprintw(app.popupWin, 6, 1, "(s) openDescriptionWindow");
    mvwprintw(app.popupWin, 7, 1, "(w) writeToFile");
    mvwprintw(app.popupWin, 8, 1, "(n) openLinkWindow");
    mvwprintw(app.popupWin, 9, 1, "(N) openLink");
}

void drawHelpPageFour() {
    wattrset(app.popupWin, COLOR_PAIR(BLUE));
    mvwprintw(app.popupWin, 1, HELP_COLS / 2 - 7, "CALENDAR HELP");
    wattrset(app.popupWin, COLOR_PAIR(0));
    mvwprintw(app.popupWin, 3, 1, "(j) prevWeek");
    mvwprintw(app.popupWin, 4, 1, "(k) nextWeek");
    mvwprintw(app.popupWin, 5, 1, "(h) prevDay");
    mvwprintw(app.popupWin, 6, 1, "(l) nextDay");
    mvwprintw(app.popupWin, 7, 1, "(</H/J) prevMonth");
    mvwprintw(app.popupWin, 8, 1, "(>/L/K) nextMonth");
    mvwprintw(app.popupWin, 9, 1, "(t) gotoToday");
    mvwprintw(app.popupWin, 10, 1, "(ENTER) pickDate");
}

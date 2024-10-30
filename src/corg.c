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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "node.h"
#include "colors.h"
#include "fileio.h"
#include "state.h"
#include "windows.h"
#include "input.h"

int main(int argc, char *argv[]) {
    initscr();
    noecho();
    curs_set(0);

    if (has_colors()) {
        start_color();

        for (int i = RED; i <= BRIGHT_WHITE; i++) {
            init_pair(i, i, COLOR_BLACK);
            init_pair(i+15, i, GRAY);
        }
        init_pair(GRAY_SEL, COLOR_BLACK, GRAY);
    }

    app.maxPriority = 4;

    Node *head = loadFromFile(argv[1]);

    app.head = head;
    app.curr = head->next;
    app.focus = Document;
    app.topLine = head->next;
    
    input.cursorPos = 0;
    strncpy(input.string, "", sizeof(input.string));

    app.isRunning = true;
    while (app.isRunning) {
        clear();
        drawDocument();

        if (app.popupWin != NULL) {
            wclear(app.popupWin);
            drawPopupWindow();
        }

        app.c = getch();
        parseInput();
    }

    freeSubtree(app.head);
    endwin();

    return EXIT_SUCCESS;
}

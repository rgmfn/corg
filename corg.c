#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "node.h"
#include "colors.h"

int main(void) {
    initscr();
    noecho();

    if (has_colors()) {
        start_color();

        for (int i = RED; i <= BRIGHT_WHITE; i++) {
            init_pair(i, i, COLOR_BLACK);
            init_pair(i+15, i, GRAY);
        }
    }

    Node head = {None, NULL, NULL, NULL, NULL};
    Node tasks = {None, "Tasks", NULL, NULL, NULL};
    Node wings = {Todo, "Get wings", NULL, NULL, NULL, NULL};
    Node reach = {Done, "Reach out to people", NULL, NULL, NULL, NULL};

    head.child = &tasks;
    tasks.child = &wings;
    wings.next = &reach;

    Node* curr = &tasks;

    printTree(&head, curr, 0);

    refresh();

    getch();

    endwin();

    return EXIT_SUCCESS;
}

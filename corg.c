#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "node.h"
#include "colors.h"
#include "fileio.h"

#define ESCAPE '\027'

int main(void) {
    initscr();
    noecho();

    if (has_colors()) {
        start_color();

        for (int i = RED; i <= BRIGHT_WHITE; i++) {
            init_pair(i, i, COLOR_BLACK);
            init_pair(i+15, i, GRAY);
        }
        init_pair(GRAY_SEL, COLOR_BLACK, GRAY);
    }

    // Node *head = loadFromFile("test.org");

    Node head = {Head};
    Node tasks = {None, "Tasks"};
    Node wings = {Todo, "Get wings"};
    Node schedule = {Todo, "Schedule wings"};
    Node eat = {Todo, "Eat"};
    Node pickup = {Todo, "Pickup"};
    Node chew = {Todo, "Chew"};
    Node reach = {Done, "Reach out to people"};
    Node ian = {None, "Ian"};
    Node megan = {None, "Megan"};
    Node helen = {None, "Helen"};
    Node moma = {None, "Moma"};

    head.next = &tasks;
    tasks.child = &wings;
    wings.child = &schedule;
    wings.parent = &tasks;
    schedule.next = &eat;
    eat.prev = &schedule;
    schedule.parent = &wings;
    eat.parent = &wings;
    eat.child = &pickup;
    pickup.next = &chew;
    chew.prev = &pickup;
    pickup.parent = &eat;
    chew.parent = &eat;
    wings.next = &reach;
    reach.prev = &wings;
    reach.child = &ian;
    reach.parent = &tasks;
    ian.next = &megan;
    megan.prev = &ian;
    ian.parent = &reach;
    megan.next = &helen;
    helen.prev = &megan;
    megan.parent = &reach;
    helen.next = &moma;
    moma.prev = &helen;
    helen.parent = &reach;
    moma.parent = &reach;

    Node *curr = head.next;

    bool run = true;
    while (run) {
        move(0, 0);
        printTree(&head, curr, 0);
        refresh();
        char c = getch();

        switch (c) {
            case 'j':
                curr = goDownVisual(curr);
                break;
            case 'k':
                curr = goUpVisual(curr);
                break;
            case 'h':
                curr = goPrevLogical(curr);
                break;
            case 'l':
                curr = goNextLogical(curr);
                break;
            case 'q':
                run = false;
                break;
            default:
                break;
        }
    }

    // freeTree(&head);
    endwin();

    return EXIT_SUCCESS;
}

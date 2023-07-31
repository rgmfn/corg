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
    curs_set(0);

    if (has_colors()) {
        start_color();

        for (int i = RED; i <= BRIGHT_WHITE; i++) {
            init_pair(i, i, COLOR_BLACK);
            init_pair(i+15, i, GRAY);
        }
        init_pair(GRAY_SEL, COLOR_BLACK, GRAY);
    }

    // Node *head = loadFromFile("test.org");

    Node head = {Head, NULL, true};
    Node tasks = {None, "Tasks", true};
    Node wings = {Todo, "Get wings", true};
    Node schedule = {Proj, "Schedule wings", true};
    Node eat = {Loop, "Eat", true};
    Node pickup = {Strt, "Pickup", true};
    Node chew = {Wait, "Chew", true};
    Node drive = { Wait, "Drive home", true};
    Node reach = {Done, "Reach out to people", true};
    Node ian = {Checked, "Ian", true};
    Node megan = {Checked, "Megan", true};
    Node helen = {Checked, "Helen", true};
    Node moma = {Checked, "Moma", true};
    Node peter = {Checked, "Peter", true};
    Node chris = {Checked, "Chris", true};
    Node victor = {Checked, "Victor", true};

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
    eat.next = &drive;
    drive.prev = &eat;
    drive.parent = &wings;
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
    moma.next = &peter;
    peter.prev = &moma;
    peter.parent = &reach;
    peter.next = &chris;
    chris.prev = &peter;
    chris.parent = &reach;
    chris.next = &victor;
    victor.prev = &chris;
    victor.parent = &reach;

    Node *curr = head.next;

    bool run = true;
    while (run) {
        clear();
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
            case 'H':
                curr = gotoParent(curr);
                break;
            case 'l':
                curr = goNextLogical(curr);
                break;
            case '\t':
                toggleSubtree(curr);
                break;
            case 'n':
                nextTodoState(curr);
                break;
            case 'p':
                prevTodoState(curr);
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

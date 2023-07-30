#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "node.h"

#define BUF_SIZE 1000

Node* loadFromFile(char* filename) {
    FILE *fp = fopen(filename, "r");

    char buffer[BUF_SIZE];

    Node *head = malloc(sizeof (Node));
    head->type = Head;

    fgets(buffer, BUF_SIZE, fp);

    // regexes?
    addstr(buffer);

    fclose(fp);

    return head;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <regex.h>

#include "node.h"

#define BUF_SIZE 1000
#define ERRBUFF_SIZE 100

#define HEADING "^(\\*+)[[:blank:]]+(([[:upper:]]{4}|\\[[X \\?\\-]\\])[[:blank:]]+)?(([[:alpha:]]+[[:blank:]]+)*[[:alpha:]]+)\n$"
/*
 * group 0: whole string
 * group 1: [****]
 * group 2: [TODO ]
 * group 3: [TODO]
 * group 4: [Get Wings]
 */
#define HEADING_GROUPS 4
#define DESCRIPTION "^(([[:alpha:]]+[[:blank:]]+)*[[:alpha:]]+)\n$"
#define DESCRIPTION_GROUPS 1

#define MAX_GROUPS 5

bool isMatch(regex_t *regex, char *string, regmatch_t *rm) {
    char errbuf[ERRBUFF_SIZE];
    int error = regexec(regex, string, HEADING_GROUPS+1, rm, 0);
    if (error == REG_NOMATCH) {
        return false;
    } else if (error) {
        regerror(error, regex, errbuf, ERRBUFF_SIZE-1);
        printw("ERROR with %s: %s\n", string, errbuf);
        exit(EXIT_FAILURE);
    }

    return true;
}

Node* placeNode(int depth, int nodeDepth, Node *curr, Node *node) {
    printw("curr: %d, node: %d", depth, nodeDepth);
    refresh();
    if (nodeDepth == depth+1) {
        curr->child = node;
        node->parent = curr;
    } else if (nodeDepth <= depth) {
        curr = riseToDepth(nodeDepth, curr);
        curr->next = node;
        node->prev = curr;
        node->parent = curr->parent;
    } else {
        printf("ERROR: Invalid depth change");
        exit(EXIT_FAILURE);
    }

    return node;
}

Node* loadFromFile(char* filename) {
    int error;
    char errbuf[ERRBUFF_SIZE];

    regmatch_t rm[MAX_GROUPS];

    regex_t heading;
    if ((error = regcomp(&heading, HEADING, REG_EXTENDED)) != 0) {
        regerror(error, &heading, errbuf, ERRBUFF_SIZE-1);
        printf("ERROR: %s:%d %s\n", __FILE__, __LINE__, errbuf);
        exit(EXIT_FAILURE);
    }

    regex_t description;
    if ((error = regcomp(&description, DESCRIPTION, REG_EXTENDED)) != 0) {
        regerror(error, &description, errbuf, ERRBUFF_SIZE-1);
        printf("ERROR: %s:%d %s\n", __FILE__, __LINE__, errbuf);
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(filename, "r");

    char buffer[BUF_SIZE];

    Node *head = malloc(sizeof (Node));
    head->type = Head;

    Node *curr = head;
    int depth = 1;

    while (fgets(buffer, BUF_SIZE, fp) != NULL) {
        printw("%s\n", buffer);

        if (isMatch(&heading, buffer, rm)) {
            Node *node = malloc(sizeof(Node));
            node->subTreeIsOpen = true;
            sprintf(node->name, "%.*s", (int)(rm[4].rm_eo-rm[4].rm_so), buffer + rm[4].rm_so);

            char typeStr[5];
            sprintf(typeStr, "%.*s", (int)(rm[3].rm_eo-rm[3].rm_so), buffer + rm[3].rm_so);
            node->type = getTypeFromString(typeStr);

            char starStr[20];
            sprintf(starStr, "%.*s", (int)(rm[1].rm_eo - rm[1].rm_so), buffer + rm[1].rm_so);
            int nodeDepth = strnlen(starStr, sizeof(starStr));

            curr = placeNode(depth, nodeDepth, curr, node);
            depth = getDepth(curr);
        } else if (isMatch(&description, buffer, rm)) {
            sprintf(curr->description, "%.*s", (int)(rm[1].rm_eo - rm[1].rm_so), buffer + rm[1].rm_so);
        } else {
            endwin();
            printf("ERROR: line type not implemented: [%s]\n", buffer);
            exit(EXIT_FAILURE);
        }
    }

    if (head->next != NULL) {
        head->next->prev = NULL;
    }

    fclose(fp);

    return head;
}

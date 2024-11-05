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
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fileio.h"
#include "node.h"
#include "state.h"
#include "util.h"

#define BUF_SIZE 1000
#define ERRBUFF_SIZE 100

#define HEADING                                                                \
    "^(\\*+)(?:[[:blank:]]+([[:upper:]]{4}|\\[[X \\?\\-]\\]))?"                \
    "(?:[[:blank:]]+(\\[\\#[A-Z]\\]))?(?:[[:blank:]]+(.+?))?"                  \
    "(?:[[:blank:]]+(\\[[[:digit:]]+\\/[[:digit:]]+\\]))?\\n$"

/*
 * group 0: whole string
 * group 1: {****}
 * group 2: {STRT}
 * group 3: {[#C]}
 * group 4: {Get some wings}
 * group 5: {[3/4]}
 */
#define HEADING_GROUPS 6

#define DESCRIPTION "^(([[:alnum:]]+[[:blank:]]+)*[[:alnum:]]+)\n$"
#define DESCRIPTION_GROUPS 1

#define TIMESTAMP                                                              \
    "^<([[:digit:]]{4})-([[:digit:]]{2})-([[:digit:]]{2}) "                    \
    "([[:alpha:]]{3})>\n$"
#define TIMESTAMP_GROUPS 4

#define DEADLINE                                                               \
    "^DEADLINE: <([[:digit:]]{4})-([[:digit:]]{2})-([[:digit:]]{2}) "          \
    "([[:alpha:]]{3})>\n$"
#define DEADLINE_GROUPS 4

#define SCHEDULED                                                              \
    "^SCHEDULED: <([[:digit:]]{4})-([[:digit:]]{2})-([[:digit:]]{2}) "         \
    "([[:alpha:]]{3})>\n$"
#define SCHEDULED_GROUPS 4

#define CLOSED                                                                 \
    "^CLOSED: \\[([[:digit:]]{4})-([[:digit:]]{2})-([[:digit:]]{2}) "          \
    "([[:alpha:]]{3})\\]\n$"
#define CLOSED_GROUPS 4

#define INACTIVE                                                               \
    "^\\[([[:digit:]]{4})-([[:digit:]]{2})-([[:digit:]]{2}) "                  \
    "([[:alpha:]]{3})\\]\n$"
#define INACTIVE_GROUPS 4

#define HYPERLINK                                                              \
    "^(((https?|ftp|smtp):\\/\\/)?(www.)?[a-z0-9]+\\.[a-z]+(\\/"               \
    "[a-zA-Z0-9#]+\\/?)*\\/?)\n$"
#define HYPERLINK_GROUPS 1

#define MAX_GROUPS 7

bool isMatch(regex_t *regex, char *string, regmatch_t *rm) {
    char errbuf[ERRBUFF_SIZE];
    int error = regexec(regex, string, HEADING_GROUPS + 1, rm, 0);
    if (error == REG_NOMATCH) {
        return false;
    } else if (error) {
        regerror(error, regex, errbuf, ERRBUFF_SIZE - 1);
        errorAndExit(errbuf);
    }

    return true;
}

/**
 * depth - depth of curr node
 * nodeDepth - depth of node to be placed
 */
Node *placeNode(int depth, int nodeDepth, Node *curr, Node *node) {
    printw("curr: %d, node: %d", depth, nodeDepth);
    refresh();
    if (nodeDepth == depth + 1) {
        curr->child = node;
        node->parent = curr;
    } else if (nodeDepth <= depth) {
        curr = riseToStarDepth(nodeDepth, curr);
        curr->next = node;
        node->prev = curr;
        node->parent = curr->parent;
    } else {
        errorAndExitf("Invalid depth change, grew by more than 1", curr->name);
        /* errorAndExitInt(depth); */
    }

    return node;
}

struct tm *getTmFromRegex(char *buffer, regmatch_t *rm) {
    char yearStr[5];
    sprintf(yearStr, "%.*s", (int)(rm[1].rm_eo - rm[1].rm_so),
            buffer + rm[1].rm_so);
    int year = atoi(yearStr);

    char monStr[3];
    sprintf(monStr, "%.*s", (int)(rm[2].rm_eo - rm[2].rm_so),
            buffer + rm[2].rm_so);
    int month = atoi(monStr);

    char dayStr[3];
    sprintf(dayStr, "%.*s", (int)(rm[3].rm_eo - rm[3].rm_so),
            buffer + rm[3].rm_so);
    int day = atoi(dayStr);

    char wdayStr[4];
    sprintf(wdayStr, "%.*s", (int)(rm[4].rm_eo - rm[4].rm_so),
            buffer + rm[4].rm_so);
    int wday = getIntFromWeekday(wdayStr);

    struct tm *timestamp = malloc(sizeof(struct tm));
    timestamp->tm_year = year - 1900;
    timestamp->tm_mon = month - 1; // 0-indexed
    timestamp->tm_mday = day;
    timestamp->tm_wday = wday;

    return timestamp;
}

Node *loadFromFile(char *filename) {
    int error;
    char errbuf[ERRBUFF_SIZE];

    regmatch_t rm[MAX_GROUPS];

    regex_t heading;
    if ((error = regcomp(&heading, HEADING, REG_EXTENDED | REG_ENHANCED)) !=
        0) {
        regerror(error, &heading, errbuf, ERRBUFF_SIZE - 1);
        errorAndExitf(errbuf, "heading regex");
    }

    regex_t description;
    if ((error = regcomp(&description, DESCRIPTION, REG_EXTENDED)) != 0) {
        regerror(error, &description, errbuf, ERRBUFF_SIZE - 1);
        errorAndExitf(errbuf, "description regex");
    }

    regex_t timestamp;
    if ((error = regcomp(&timestamp, TIMESTAMP, REG_EXTENDED)) != 0) {
        regerror(error, &timestamp, errbuf, ERRBUFF_SIZE - 1);
        errorAndExitf(errbuf, "timestamp regex");
    }

    regex_t deadline;
    if ((error = regcomp(&deadline, DEADLINE, REG_EXTENDED)) != 0) {
        regerror(error, &deadline, errbuf, ERRBUFF_SIZE - 1);
        errorAndExitf(errbuf, "deadline regex");
    }

    regex_t scheduled;
    if ((error = regcomp(&scheduled, SCHEDULED, REG_EXTENDED)) != 0) {
        regerror(error, &scheduled, errbuf, ERRBUFF_SIZE - 1);
        errorAndExitf(errbuf, "scheduled regex");
    }

    regex_t closed;
    if ((error = regcomp(&closed, CLOSED, REG_EXTENDED)) != 0) {
        regerror(error, &closed, errbuf, ERRBUFF_SIZE - 1);
        errorAndExitf(errbuf, "closed regex");
    }

    regex_t inactive;
    if ((error = regcomp(&inactive, INACTIVE, REG_EXTENDED)) != 0) {
        regerror(error, &inactive, errbuf, ERRBUFF_SIZE - 1);
        errorAndExitf(errbuf, "inactive regex");
    }

    regex_t hyperlink;
    if ((error = regcomp(&hyperlink, HYPERLINK, REG_EXTENDED)) != 0) {
        regerror(error, &hyperlink, errbuf, ERRBUFF_SIZE - 1);
        errorAndExitf(errbuf, "hyperlink regex");
    }

    Node *head = malloc(sizeof(Node));
    head->type = Head;

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        Node *next = malloc(sizeof(Node));
        next->type = None;
        next->subtreeType = InfoAndSubtree;
        head->next = next;
        next->priority = -1;

        strncpy(app.filename, "", sizeof(app.filename));

        return head;
    }

    strncpy(app.filename, filename, sizeof(app.filename));

    char buffer[BUF_SIZE];

    Node *curr = head;
    int depth = 1;

    while (fgets(buffer, BUF_SIZE, fp) != NULL) {
        printw("%s\n", buffer);

        if (isMatch(&heading, buffer, rm)) {
            Node *node = malloc(sizeof(Node));
            node->subtreeType = InfoAndSubtree;
            sprintf(node->name, "%.*s", (int)(rm[4].rm_eo - rm[4].rm_so),
                    buffer + rm[4].rm_so);

            char typeStr[5];
            sprintf(typeStr, "%.*s", (int)(rm[2].rm_eo - rm[2].rm_so),
                    buffer + rm[2].rm_so);
            node->type = getTypeFromString(typeStr);

            char priorityStr[5];
            sprintf(priorityStr, "%.*s", (int)(rm[3].rm_eo - rm[3].rm_so),
                    buffer + rm[3].rm_so);
            if (priorityStr[2] >= 'A' && priorityStr[2] <= 'Z') {
                node->priority = priorityStr[2] - 'A';
            } else {
                node->priority = -1;
            }

            if (node->priority > app.maxPriority) {
                app.maxPriority = node->priority;
            }

            char starStr[20];
            /* TODO; // will break past 19 indents */
            sprintf(starStr, "%.*s", (int)(rm[1].rm_eo - rm[1].rm_so),
                    buffer + rm[1].rm_so);
            int nodeDepth = strnlen(starStr, sizeof(starStr));
            /* int nodeDepth = (int)(rm[1].rm_eo - rm[1].rm_so); */
            // can I do math instead??

            char counterStr[10];
            sprintf(counterStr, "%.*s", (int)(rm[5].rm_eo - rm[5].rm_so),
                    buffer + rm[5].rm_so);
            if (strnlen(counterStr, sizeof(counterStr)) > 0) {
                node->hasCounter = true;
            } else {
                node->hasCounter = false;
            }

            curr = placeNode(depth, nodeDepth, curr, node);
            depth = getStarDepth(curr);
        } else if (isMatch(&timestamp, buffer, rm)) {
            curr->date = getTmFromRegex(buffer, rm);
            curr->dateType = Timestamp;
        } else if (isMatch(&deadline, buffer, rm)) {
            curr->date = getTmFromRegex(buffer, rm);
            curr->dateType = Deadline;
        } else if (isMatch(&scheduled, buffer, rm)) {
            curr->date = getTmFromRegex(buffer, rm);
            curr->dateType = Scheduled;
        } else if (isMatch(&inactive, buffer, rm)) {
            curr->date = getTmFromRegex(buffer, rm);
            curr->dateType = Inactive;
        } else if (isMatch(&closed, buffer, rm)) {
            curr->date = getTmFromRegex(buffer, rm);
            curr->dateType = Closed;
        } else if (isMatch(&hyperlink, buffer, rm)) {
            sprintf(curr->link, "%.*s", (int)(rm[1].rm_eo - rm[1].rm_so),
                    buffer + rm[1].rm_so);
        }
        // DESCRIPTION MUST GO AT END, WILL CAPTURE ANYTHING
        else if (isMatch(&description, buffer, rm)) {
            sprintf(curr->description, "%.*s", (int)(rm[1].rm_eo - rm[1].rm_so),
                    buffer + rm[1].rm_so);
        }
    }

    if (head->next != NULL) {
        head->next->prev = NULL;
    }

    fclose(fp);

    regfree(&heading);
    regfree(&description);
    regfree(&timestamp);
    regfree(&deadline);
    regfree(&scheduled);

    return head;
}

void printSubtreeToFile(Node *node, FILE *fp) {
    if (node == NULL)
        return;

    int depth = getStarDepth(node);

    for (int i = 0; i < depth; i++) {
        fprintf(fp, "*");
    };

    if (node->type != None) {
        fprintf(fp, " %s", getTypeStr(node->type));
    }

    if (node->priority > 0) {
        fprintf(fp, " [#%c]", 'A' + node->priority);
    }

    if (strlen(node->name) > 0) {
        fprintf(fp, " %s", node->name);
    }

    fputc('\n', fp);

    if (node->date != NULL) {
        switch (node->dateType) {
        case Deadline:
            fprintf(fp, "DEADLINE: ");
            break;
        case Scheduled:
            fprintf(fp, "SCHEDULED: ");
            break;
        case Closed:
            fprintf(fp, "CLOSED: ");
            break;
        case Inactive:
        case Timestamp:
            break;
        }

        char *dateStr = tmToString(node->date, node->dateType);
        fprintf(fp, "%s\n", dateStr);
        free(dateStr);
    }

    if (strnlen(node->description, sizeof(node->description)) > 0) {
        fprintf(fp, "%s\n", node->description);
    }

    if (strnlen(node->link, sizeof(node->link)) > 0) {
        fprintf(fp, "%s\n", node->link);
    }

    if (node->hasCounter) {
        int numAnyTodo = countAnyTodo(node->child);
        int numDoneTodo = countDoneTodo(node->child);
        fprintf(fp, " [%d/%d]", numDoneTodo, numAnyTodo);
    }

    printSubtreeToFile(node->child, fp);
    printSubtreeToFile(node->next, fp);
}

void writeToFile(Node *node, char *filename) {
    FILE *fp = fopen(filename, "w");

    printSubtreeToFile(node, fp);

    fclose(fp);
}

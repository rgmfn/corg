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
#include <string.h>
#include <stdlib.h>

#include "node.h"
#include "colors.h"
#include "util.h"
#include "windows.h"
#include "state.h"

// === private methods === {{{

int getDepthColor(int depth) {
    switch (depth%3) {
        case 0:
            return BLUE;
        case 1:
            return MAGENTA;
        case 2:
            return CYAN;
        default:
            return WHITE;
    }
}

char getBulletChar(NodeType type) {
    // switch (type) {
    //     default:
    //         return '*';
    // }

    return '*'; // no other types implemented yet
}

Node* runDownBack(Node *curr) {
    if (curr->next != NULL)
        return runDownBack(curr->next);
    else if (curr->child != NULL) {
        return runDownBack(curr->child);
    }

    return curr;
}

Node* runToBack(Node *node) {
    if (node->next == NULL) {
        return node;
    }

    return runToBack(node->next);
}

Node* runToBackAndMakeParent(Node *node, Node *newParent) {
    if (node->next == NULL) {
        node->parent = newParent;

        return node;
    }

    node->parent = newParent;

    return runToBackAndMakeParent(node->next, newParent);
}

Node* runToFront(Node *node) {
    if (node->prev == NULL) {
        return node;
    }

    return runToFront(node->prev);
}

// === end of private methods === }}}

char* getTypeStr(NodeType type) {
    // space after is baked in so that space doesn't appear for no type
    switch (type) {
        case None:
            return "";
        case Todo:
            return "TODO ";
        case Proj:
            return "PROJ ";
        case Loop:
            return "LOOP ";
        case Strt:
            return "STRT ";
        case Wait:
            return "WAIT ";
        case Hold:
            return "HOLD ";
        case Idea:
            return "IDEA ";
        case Done:
            return "DONE ";
        case Kill:
            return "KILL ";
        case Unchecked:
            return "[ ] ";
        case Started:
            return "[-] ";
        case Waiting:
            return "[?] ";
        case Checked:
            return "[X] ";
        case Okay:
            return "OKAY ";
        case Yes:
            return "YES ";
        case No:
            return "NO ";
        case Head:
            return "HEAD ";
        default:
            return "ERROR ";
    }
}

NodeType getTypeFromString(char *string) {
    if (strcmp(string, "") == 0) {
        return None;
    } else if (strcmp(string, "TODO") == 0) {
        return Todo;
    } else if (strcmp(string, "PROJ") == 0) {
        return Proj;
    } else if (strcmp(string, "LOOP") == 0) {
        return Loop;
    } else if (strcmp(string, "STRT") == 0) {
        return Strt;
    } else if (strcmp(string, "WAIT") == 0) {
        return Wait;
    } else if (strcmp(string, "HOLD") == 0) {
        return Hold;
    } else if (strcmp(string, "IDEA") == 0) {
        return Idea;
    } else if (strcmp(string, "DONE") == 0) {
        return Done;
    } else if (strcmp(string, "KILL") == 0) {
        return Kill;
    } else if (strcmp(string, "[ ]") == 0) {
        return Unchecked;
    } else if (strcmp(string, "[-]") == 0) {
        return Started;
    } else if (strcmp(string, "[?]") == 0) {
        return Waiting;
    } else if (strcmp(string, "[X]") == 0) {
        return Checked;
    } else if (strcmp(string, "OKAY") == 0) {
        return Okay;
    } else if (strcmp(string, "YES") == 0) {
        return Yes;
    } else if (strcmp(string, "NO") == 0) {
        return No;
    }

    return Head;
}

bool isFinishedType(NodeType type) {
    switch (type) {
        case Done:
        case Kill:
        case Checked:
        case Okay:
        case Yes:
        case No:
            return true;
        default:
            return false;
    }
}

void printNode(Node* node, int depth) {
    if (node->type == Head) return;

    int textColor = getDepthColor(depth);
    int offset = 0;

    int typeColor = getTypeColor(node->type);
    char* typeStr = getTypeStr(node->type);
    char bulletChar = getBulletChar(node->type);

    windentNTimes(stdscr, depth);

    if (typeColor == GRAY) {
        textColor = GRAY;
    }

    if (node == app.curr) {
        offset = 15;
    }
    textColor += offset;
    typeColor += offset;

    attrset(COLOR_PAIR(textColor));
    addch(bulletChar);
    addch(' ');

    attrset(COLOR_PAIR(typeColor));
    addstr(typeStr);

    attrset(COLOR_PAIR(textColor));
    addstr(node->name);

    if (node->hasCounter) {
        int numAnyTodo = countAnyTodo(node->child);
        int numDoneTodo = countDoneTodo(node->child);

        if (numAnyTodo == numDoneTodo) {
            attrset(COLOR_PAIR(GRAY+offset));
        } else {
            attrset(COLOR_PAIR(GREEN+offset));
        }
        printw(" [%d/%d]", numDoneTodo, numAnyTodo);
    }

    attrset(COLOR_PAIR(0));
    if (!node->subTreeIsOpen) {
        addstr(" ...\n");
        return;
    }

    if (node->date != NULL) {
        addch('\n');
        windentNTimes(stdscr, depth+1);

        attrset(COLOR_PAIR(GRAY));

        char *dateStr;
        switch (node->dateType) {
            case Timestamp:
            case Inactive:
                break;
            case Deadline:
                addstr("DEADLINE: ");
                break;
            case Scheduled:
                addstr("SCHEDULED: ");
                break;
            case Closed:
                addstr("CLOSED: ");
                break;
        }

        attrset(COLOR_PAIR(YELLOW));
        dateStr = tmToString(node->date, node->dateType);
        addstr(dateStr);
        free(dateStr);

        attrset(COLOR_PAIR(0));
    }

    if (strnlen(node->description, sizeof(node->description)) > 0) {
        addch('\n');
        windentNTimes(stdscr, depth+1);
        addstr(node->description);
    }

    if (strnlen(node->link, sizeof(node->link)) > 0) {
        attrset(COLOR_PAIR(BLUE)|A_UNDERLINE);
        addch('\n');
        windentNTimes(stdscr, depth+1);
        addstr(node->link);
        attrset(COLOR_PAIR(0));
    }

    addch('\n');
}

void printTree(Node *node, int depth) {
    if (node == NULL) return; // no tree to draw

    printNode(node, depth);

    printTree(node->child, depth+1);
    printTree(node->next, depth);
}

void printPartialTree(Node *node, int nodesToDraw) {
    if (node == NULL || nodesToDraw <= 0) return;

    printNode(node, getDepth(node));

    int lines = 1; // * STRT thing

    if (strnlen(node->description, sizeof(node->description)) > 0) {
        lines++;
    }
    
    if (node->date != NULL) {
        lines++; 
    }

    printPartialTree(goDownVisualOrNull(node), nodesToDraw-lines);
}

void freeSubtree(Node *node) {
    if (node == NULL) {
        return;
    }

    Node *child = node->child;
    Node *next = node->next;

    node->child = NULL;
    node->next = NULL;

    freeSubtree(child);
    freeSubtree(next);

    node->parent = NULL;
    node->prev = NULL;

    if (node->date != NULL) {
        free(node->date);
    }

    free(node);
}

void toggleSubtree(Node *subroot) {
    bool hasSubtree = subroot->child != NULL ||
        strnlen(subroot->description, sizeof(subroot->description)) > 0 ||
        subroot->date != NULL;
    if (!hasSubtree)
        subroot->subTreeIsOpen = true;
    else if (subroot != NULL)
        subroot->subTreeIsOpen = !subroot->subTreeIsOpen;
}

/**
 * goDownVisual except when it goes down from the last node it returns NULL
 */
Node* goDownVisualOrNull(Node *curr) {
    Node *down = goDownVisual(curr);

    if (down == curr) {
        return NULL;
    } else {
        return down;
    }
}

Node* goDownVisual(Node *curr) {
    if (curr->subTreeIsOpen && curr->child != NULL)
        return curr->child;
    else if (curr->next != NULL)
        return curr->next;

    Node *parent = curr->parent;
    while (parent != NULL && parent->next == NULL) {
        parent = parent->parent;
    }

    if (parent == NULL) {
        return curr;
    }

    return parent->next;
}

Node* goUpVisual(Node *curr) {
    if (curr->prev != NULL && curr->prev->child != NULL &&
        curr->prev->subTreeIsOpen)
        return runDownBack(curr->prev->child);
    else if (curr->prev != NULL)
        return curr->prev;
    else if (curr->parent != NULL)
        return curr->parent;

    return curr;
}

Node* goNextLogical(Node *curr) {
    if (curr->next != NULL)
        return curr->next;

    return goDownVisual(curr);
}

Node* goPrevLogical(Node *curr) {
    if (curr->prev != NULL)
        return curr->prev;

    return goUpVisual(curr);
}

Node* gotoParent(Node *curr) {
    if (curr->parent != NULL) {
        return curr->parent;
    }

    return curr;
}

#define SCROLL_OFFSET 4

void tryScrollUp(Node *curr) {
    int distance;
    if (isAbove(app.topLine, curr)) {
        distance = getVisualDistance(app.topLine, curr) * -1;
    } else {
        distance = getVisualDistance(curr, app.topLine);
    }

    int moveBy = distance + SCROLL_OFFSET;

    for (int i = 0; i < moveBy; i++) {
        app.topLine = goUpVisual(app.topLine);
    }
}

void tryScrollDown(Node *curr) {
    int distance = getVisualDistance(app.topLine, curr);
    int size = getVisualSize(curr) + SCROLL_OFFSET;

    int moveBy = distance + size - LINES;

    for (int i = 0; i < moveBy; i++) {
        app.topLine = goDownVisual(app.topLine);
    }
}

int getVisualSize(Node *node) {
    int lines = 1; // * DONE task

    if (strnlen(node->description, sizeof(node->description)) > 0) {
        lines++;
    }

    if (strnlen(node->link, sizeof(node->link)) > 0) {
        lines++;
    }

    if (node->date != NULL) {
        lines++;
    }

    return lines;
}

int getVisualDistance(Node* node, Node *bottom) {
    if (node == NULL) {
        errorAndExit("top node is below bottom node");
    }

    if (node == bottom) {
        return 0;
    }

    if (!node->subTreeIsOpen) {
        return 1;
    }

    return getVisualSize(node) + getVisualDistance(goDownVisualOrNull(node), bottom);
}

bool isAbove(Node *node, Node *other) {
    Node *next = goDownVisualOrNull(node);

    if (!next) {
        return false;
    } else if (node == other) {
        return true;
    }

    return isAbove(next, other);
}

Node* riseToStarDepth(int targetDepth, Node *node) {
    if (node == NULL) {
        errorAndExit("Tried to reach depth 0");
    }

    int currentDepth = getStarDepth(node);

    if (currentDepth <= targetDepth) {
        return node;
    } else {
        return riseToStarDepth(targetDepth, node->parent);
    }
}

/**
 * depth - the number of indents ('  ') before a line,
 *         used for node drawing
 * star depth - the number of stars on a line
 *              used for fileio
 *
 * star depth = depth + 1
 */
int getStarDepth(Node *node) {
    if (node == NULL)
        return 0;

    return 1 + getStarDepth(node->parent);
}

int getDepth(Node *node) {
    if (node == NULL)
        return -1;

    return 1 + getDepth(node->parent);
}

void createChildNode(Node *subroot) {
    Node *child = malloc(sizeof(Node));
    child->type = None;
    strncpy(child->name, "", sizeof(child->name));
    strncpy(child->description, "", sizeof(child->description));
    child->subTreeIsOpen = true;

    if (subroot->child != NULL) {
        child->next = subroot->child;
        child->next->prev = child;
    }
    subroot->child = child;
    child->parent = subroot;

    app.curr = child;
}

void createSiblingNodeAfter(Node *subroot) {
    Node *sibling = malloc(sizeof(Node));
    sibling->type = None;
    strncpy(sibling->name, "", sizeof(sibling->name));
    strncpy(sibling->description, "", sizeof(sibling->description));
    sibling->subTreeIsOpen = true;

    if (subroot->next != NULL) {
        sibling->next = subroot->next;
        sibling->next->prev = sibling;
    }

    subroot->next = sibling;
    sibling->prev = subroot;
    sibling->parent = subroot->parent;

    app.curr = sibling;
}

void deleteNode(Node *subroot) {
    // subroot is root, with next
    // h---x---*
    //      \
    //       *
    if (subroot->prev == NULL && subroot->parent == NULL &&
            subroot->next != NULL) {
        Node *head = app.head;
        Node *next = subroot->next;

        next->prev = NULL;
        head->next = next;
        subroot->next = NULL; // so we don't free next

        app.curr = next;

        freeSubtree(subroot);

        createSiblingNodeAfter(app.head);
        app.topLine = app.head->next;
    }
    // subroot is root, no next
    // h---x
    //      \
    //       *
    else if (subroot->prev == NULL && subroot->parent == NULL &&
            subroot->next == NULL) {
        app.head->next = NULL;

        app.curr = NULL;

        freeSubtree(subroot);

        createSiblingNodeAfter(app.head);
        app.head->next->prev = NULL;
        app.topLine = app.head->next;
    }
    // subroot is the first child of it's parent, with next
    // *
    //  \
    //   x---*
    //    \
    //     *
    else if (subroot->next != NULL && subroot->prev == NULL &&
            subroot->parent != NULL) {
        Node *parent = subroot->parent;
        Node *next = subroot->next;

        next->prev = NULL;
        parent->child = next;
        subroot->next = NULL; // so that we don't free next

        app.curr = next;

        freeSubtree(subroot);
    }
    // first child of parent, no next
    // *
    //  \
    //   x
    //    \
    //     *
    else if (subroot->next == NULL && subroot->prev == NULL &&
            subroot->parent != NULL) {
        Node *parent = subroot->parent;

        parent->child = NULL;

        app.curr = parent;

        freeSubtree(subroot);
    }
    // not parent's child, no next
    // *
    //  \
    //   *---x
    //    \   \
    //     *   *
    else if (subroot->parent != NULL && subroot->parent->child != subroot &&
            subroot->prev != NULL && subroot->next == NULL) {
        Node *prev = subroot->prev;

        prev->next = NULL;

        app.curr = prev;

        freeSubtree(subroot);
    }
    // normally
    // *
    //  \
    //   *---x---*---
    //    \   \   \
    //     *   *   *
    else {
        Node *prev = subroot->prev;
        Node *next = subroot->next;

        prev->next = next;
        prev->next->prev = prev;
        subroot->next = NULL; // so that we don't free next

        app.curr = next;

        freeSubtree(subroot);
    }
}

/**
 * *---x        *      
 *      \   ->   \
 *       *        x---*
 *
 * sibling -> child
 *
 * * Thing -> * Thing
 * * Thing      * Thing
 *
 * Popping out refers to the node graph, not the visual motion in the app.
 */
void tryPopNodeOut(Node *node) {
    Node *prev = node->prev;
    Node *next = node->next;
    Node *child = node->child;

    if (prev == NULL) {
        return;
    }

    if (next != NULL) {
        prev->next = next;
        next->prev = prev;
    } else {
        prev->next = NULL;
    }

    if (child != NULL) {
        node->next = child;
        child->prev = node;
        (void) runToBackAndMakeParent(child, prev);
    } else {
        node->next = NULL;
    }

    node->parent = prev;
    node->child = NULL;

    if (prev->child == NULL) {
        prev->child = node;
        node->prev = NULL;
    } else {
        Node *back = runToBack(prev->child);
        back->next = node;
        node->prev = back;
    }
}

/**
 * *            *---x
 *  \       ->       \
 *   x---*            *
 *
 * child -> sibling
 *
 * * Thing   -> * Thing
 *   * Thing    * Thing
 *
 * Popping in refers to the node graph, not the visual motion in the app.
 */
void tryPopNodeIn(Node *node) {
    Node *next = node->next;
    Node *parent = node->parent;

    if (parent == NULL || node->prev != NULL || node->child != NULL) {
        return;
    }

    Node *parentNext = node->parent->next;

    parent->next = node;
    parent->child = NULL;

    node->parent = parent->parent;
    node->prev = parent;

    if (parentNext != NULL) {
        node->next = parentNext;
        parentNext->prev = node;
    } else {
        node->next = NULL;
    }

    if (next != NULL) {
        node->child = next;
        next->prev = NULL;

        runToBackAndMakeParent(next, node);
    } else {
        node->child = NULL;
    }
}

void moveNodeDown(Node *node) {
    Node *prev = node->prev;
    Node *next = node->next;
    Node *parent = node->parent;

    if (next == NULL) {
        return;
    }

    Node *nextNext = next->next;

    node->next = nextNext;
    if (nextNext != NULL)
        nextNext->prev = node;

    if (prev != NULL)
        prev->next = next;
    next->prev = prev;

    node->prev = next;
    next->next = node;

    if (parent != NULL && parent->child == node) {
        parent->child = next;
    }
}

/*
 * *---*---x---*
 */
void moveNodeUp(Node *node) {
    Node *prev = node->prev;
    Node *next = node->next;

    if (prev == NULL) {
        return;
    }

    Node *prevParent = prev->parent;
    Node *prevPrev = prev->prev;

    if (prevPrev != NULL)
        prevPrev->next = node;
    node->prev = prevPrev;

    prev->next = next;
    if (next != NULL)
        next->prev = prev;

    node->next = prev;
    prev->prev = node;

    if (prevParent != NULL && prevParent->child == prev) {
        prevParent->child = node;
    }
}

void toggleCounter(Node *node) {
    node->hasCounter = !node->hasCounter;
}

/**
 * Counts the number of nodes with any todo status. Searches current node
 *  and any nodes past it (node->next, node->next->next, etc.).
 */
int countAnyTodo(Node *node) {
    if (node == NULL) {
        return 0;
    }

    if (node->type != None && node->type != Head) {
        return 1 + countAnyTodo(node->next);
    }

    return countAnyTodo(node->next);
}

/**
 * Counts the number of nodes with any 'completed' status. Searches current
 *  node and any nodes past it (node->next, node->next->next, etc.).
 */
int countDoneTodo(Node *node) {
    if (node == NULL) {
        return 0;
    }

    if (isFinishedType(node->type)) {
        return 1 + countDoneTodo(node->next);
    }

    return countDoneTodo(node->next);
}

NodeType cycleNodeType(NodeType type) {
    switch (type) {
        case Todo:
            return Strt;
        case Strt:
            return Done;
        case Done:
            return Todo;
        case Unchecked:
            return Started;
        case Started:
            return Checked;
        case Checked:
            return Unchecked;
        case Okay:
            return Yes;
        case Yes:
            return No;
        case No:
            return Okay;
        default:
            break;
    }

    return type;
}

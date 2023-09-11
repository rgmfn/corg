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

/**
 * TODO make runToBackAnd(Node *node, void (*fun)()) function?
 */
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
    // TODO better solution than space after type?
    // so that space doesn't appear for no type
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

void printNode(Node* node, int depth) {
    if (node->type == Head) return;

    int textColor = getDepthColor(depth);

    int typeColor = getTypeColor(node->type);
    char* typeStr = getTypeStr(node->type);
    char bulletChar = getBulletChar(node->type);

    windentNTimes(stdscr, depth);

    if (typeColor == GRAY) {
        textColor = GRAY;
    }

    if (node == app.curr) {
        textColor += 15;
        typeColor += 15;
    }

    attrset(COLOR_PAIR(textColor));
    addch(bulletChar);
    addch(' ');

    attrset(COLOR_PAIR(typeColor));
    addstr(typeStr);

    attrset(COLOR_PAIR(textColor));
    addstr(node->name);

    attrset(COLOR_PAIR(0));
    if (!node->subTreeIsOpen) {
        addstr(" ...\n");
    } else if (strnlen(node->description, sizeof(node->description)) > 0) {
        addch('\n');
        windentNTimes(stdscr, depth+1);
        addstr(node->description);
        addch('\n');
    } else {
        addch('\n');
    }
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
    
    // TODO add to lines when dates are a thing

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

    free(node);
}

void toggleSubtree(Node *subroot) {
    bool hasSubtree = subroot->child != NULL ||
        strnlen(subroot->description, sizeof(subroot->description)) > 0;
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
    if (getVisualDistance(app.topLine, curr) < SCROLL_OFFSET) {
        app.topLine = goUpVisual(app.topLine);
    }
}

void tryScrollDown(Node *curr) {
    if (getVisualDistance(app.topLine, curr) > (LINES - SCROLL_OFFSET - 1)) {
        app.topLine = goDownVisual(app.topLine);
    }
}

int getVisualDistance(Node* node, Node *bottom) {
    if (node == NULL) {
        errorAndExit("top node is below bottom node");
    }

    if (node == bottom) {
        return 0;
    }

    int lines = 1; // * DONE task

    if (strnlen(node->description, sizeof(node->description)) > 0) {
        lines++;
    }

    // TODO add to lines when dates are a thing

    return lines + getVisualDistance(goDownVisualOrNull(node), bottom);
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

/**
 * TODO what to do about overwriting current child tree?
 * make child after all current children?
 */
void createChildNode(Node *subroot) {
    Node *child = malloc(sizeof(Node));
    child->type = None;
    strncpy(child->name, "", sizeof(child->name));
    strncpy(child->description, "", sizeof(child->description));
    child->subTreeIsOpen = true;

    if (subroot->next != NULL) {
        child->next = subroot->child;
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

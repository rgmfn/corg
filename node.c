#include <curses.h>
#include <string.h>
#include <stdlib.h>

#include "node.h"
#include "colors.h"
#include "windows.h"

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

int getTypeColor(NodeType type) {
    switch (type) {
        case Todo:
        case Loop:
        case Idea:
        case Proj:
        case Unchecked:
            return GREEN;
        case Done:
        case Yes:
        case Okay:
        case Checked:
            return GRAY;
        case Wait:
        case Hold:
        case Waiting:
            return YELLOW;
        case Kill:
        case No:
            return RED;
        case Strt:
        case Started:
            return MAGENTA;
        default:
            return 0;
    }
}

void printNode(Node* node, Node* curr, int depth) {
    if (node->type == Head) return;

    int textColor = getDepthColor(depth);

    int typeColor = getTypeColor(node->type);
    char* typeStr = getTypeStr(node->type);
    char bulletChar = getBulletChar(node->type);

    indentNTimes(stdscr, depth);

    if (typeColor == GRAY) {
        textColor = GRAY;
    }

    if (node == curr) {
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
        indentNTimes(stdscr, depth+1);
        addstr(node->description);
        addch('\n');
    } else {
        addch('\n');
    }
}

void printTree(Node* node, Node* curr, int depth) {
    if (node == NULL) return; // no tree to draw

    printNode(node, curr, depth);

    if (node->subTreeIsOpen && node->child != NULL)
        printTree(node->child, curr, depth+1);

    if (node->next != NULL)
        printTree(node->next, curr, depth);
}

// TODO make sure this is correct
void freeTree(Node *node) {
    if (node == NULL) {
        return;
    }

    Node *child = node->child;
    Node *next = node->next;

    node->child = NULL;
    node->next = NULL;

    freeTree(child);
    freeTree(next);

    node->parent = NULL;

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

Node* runDownBack(Node *curr) {
    if (curr->next != NULL)
        return runDownBack(curr->next);
    else if (curr->child != NULL) {
        return runDownBack(curr->child);
    }

    return curr;
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

#include <curses.h>
#include <string.h>
#include <stdlib.h>

#include "node.h"
#include "colors.h"
#include "util.h"
#include "windows.h"
#include "state.h"

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

Node* riseToDepth(int targetDepth, Node *node) {
    if (node == NULL) {
        errorAndExit("Tried to reach depth 0");
    }

    int currentDepth = getDepth(node);

    if (currentDepth <= targetDepth) {
        return node;
    } else {
        return riseToDepth(targetDepth, node->parent);
    }
}

int getDepth(Node *node) {
    if (node == NULL)
        return 0;

    return 1 + getDepth(node->parent);
}

/**
 * TODO what to do about overwriting current child tree?
 * make child after all current children
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

/**
 * TODO
 * or child of parent node
 */
void deleteNode(Node *subroot) {
    // if subroot has next and is child of parent
    if (subroot->parent != NULL && subroot->parent->child == subroot &&
            subroot->next != NULL)
    {
        Node *next = subroot->next;
        Node *parent = subroot->parent;

        parent->child = next;
        next->prev = NULL;

        freeSubtree(subroot);
    }
 
    // if subroot is root (no next)
    else if (subroot == app.head->next && subroot->next == NULL) {
        app.head->next = NULL;
        subroot->prev = NULL;

        freeSubtree(subroot);
    }

    // if subroot is root (with next)
    else if (subroot == app.head->next) {
        app.head->next = subroot->next;
        app.head->next->prev = app.head;

        freeSubtree(subroot);
    }
 
    // other
    else {
        subroot->prev->next = subroot->next;
        subroot->prev->next->prev = subroot->prev;

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

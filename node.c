#include <curses.h>

#include "node.h"
#include "colors.h"

int getDepthColor(int depth) {
    switch (depth) {
        case 1:
            return BLUE;
        case 2:
            return MAGENTA;
        case 3:
            return CYAN;
        default:
            return RED;
    }
}

char getBulletChar(NodeType type) {
    switch (type) {
        case Todo:
        case Done:
        case None:
            return '*';
        default:
            return '\0';
    }
}

char* getTypeStr(NodeType type) {
    // TODO better solution than space after type?
    // so that space doesn't appear for no type
    switch (type) {
        case None:
            return "";
        case Todo:
            return "TODO ";
        case Done:
            return "DONE ";
        default:
            return "ERROR ";
    }
}

int getTypeColor(NodeType type) {
    switch (type) {
        case Todo:
            return GREEN;
        case Done:
            return GRAY;
        default:
            return 0;
    }
}

void printNode(Node* node, Node* curr, int depth) {
    int textColor = getDepthColor(depth);

    int typeColor = getTypeColor(node->type);
    char* typeStr = getTypeStr(node->type);
    char bulletChar = getBulletChar(node->type);

    for (int i = 0; i < depth; i++) {
        addstr("  ");
    }

    if (node == curr) {
        textColor += 15;
        typeColor += 15;
    }

    if (node->type == Done) {
        textColor = GRAY;
    }

    if (depth == 0) {
        return; // don't draw anything for head node
    }

    attrset(COLOR_PAIR(textColor));
    addch(bulletChar);
    addch(' ');

    attrset(COLOR_PAIR(typeColor));
    addstr(typeStr);

    attrset(COLOR_PAIR(textColor));
    addstr(node->text);

    attrset(COLOR_PAIR(0));
    addch('\n');
}

void printTree(Node* node, Node* curr, int depth) {
    printNode(node, curr, depth);

    if (node->child != NULL) {
        printTree(node->child, curr, depth+1);
    }

    if (node->next != NULL) {
        printTree(node->next, curr, depth);
    }
}

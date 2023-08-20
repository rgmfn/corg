#ifndef NODE_H
#define NODE_H

#include <stdbool.h>

typedef enum NodeType {
    Head,
    None,
    Todo,
    Proj,
    Loop,
    Strt,
    Wait,
    Hold,
    Idea,
    Done,
    Kill,
    Unchecked,
    Started,
    Waiting,
    Checked,
    Okay,
    Yes,
    No,
} NodeType;

typedef struct Node {
    NodeType type;

    char name[50];
    char description[100];

    bool subTreeIsOpen;

    struct Node* next;
    struct Node* prev;
    struct Node* child;
    struct Node* parent;
} Node;

char* getTypeStr(NodeType);
NodeType getTypeFromString(char*);

void printNode(Node*, Node*, int);
void printTree(Node*, Node*, int);

void freeSubtree(Node*);

void toggleSubtree(Node*);

Node* goDownVisual(Node*);
Node* goUpVisual(Node*);
Node* goNextLogical(Node*);
Node* goPrevLogical(Node*);
Node* gotoParent(Node*);

Node* riseToDepth(int, Node*);
int getDepth(Node*);

void createChildNode(Node*);
void createSiblingNodeAfter(Node*);
void deleteNode(Node*);

NodeType cycleNodeType(NodeType);
#endif

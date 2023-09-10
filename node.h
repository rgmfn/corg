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

void printNode(Node*, int);
void printTree(Node*, int);
void printPartialTree(Node*, int);

void freeSubtree(Node*);

void toggleSubtree(Node*);

Node* goDownVisual(Node*);
Node* goDownVisualOrNull(Node*);
Node* goUpVisual(Node*);
Node* goNextLogical(Node*);
Node* goPrevLogical(Node*);
Node* gotoParent(Node*);

void tryScrollUp(Node*);
void tryScrollDown(Node*);

int getVisualDistance(Node*, Node*);

Node* riseToStarDepth(int, Node*);
int getStarDepth(Node*);
int getDepth(Node*);

void createChildNode(Node*);
void createSiblingNodeAfter(Node*);
void deleteNode(Node*);

void tryPopNodeOut(Node*);
void tryPopNodeIn(Node*);

NodeType cycleNodeType(NodeType);
#endif

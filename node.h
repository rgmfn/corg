#ifndef NODE_H
#define NODE_H

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

    bool subTreeIsOpen;

    struct Node* next;
    struct Node* prev;
    struct Node* child;
    struct Node* parent;
} Node;

int getTypeColor(NodeType);

void printNode(Node*, Node*, int);
void printTree(Node*, Node*, int);

void freeTree(Node*);

void toggleSubtree(Node*);

Node* goDownVisual(Node*);
Node* goUpVisual(Node*);
Node* goNextLogical(Node*);
Node* goPrevLogical(Node*);
Node* gotoParent(Node*);

NodeType cycleNodeType(NodeType);
#endif

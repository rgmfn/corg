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

    char* text;

    bool subTreeIsOpen;

    struct Node* next;
    struct Node* prev;
    struct Node* child;
    struct Node* parent;
} Node;

void printNode(Node*, Node*, int);
void printTree(Node*, Node*, int);

void freeTree(Node*);

Node* goDownVisual(Node*);
Node* goUpVisual(Node*);
Node* goNextLogical(Node*);
Node* goPrevLogical(Node*);
#endif

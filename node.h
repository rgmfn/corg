typedef enum NodeType {
    None,
    Todo,
    Done,
} NodeType;

typedef struct Node {
    NodeType type;

    char* text;

    struct Node* next;
    struct Node* prev;
    struct Node* child;
    struct Node* parent;
} Node;

void printNode(Node*, Node*, int);
void printTree(Node*, Node*, int);

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

#ifndef NODE_H
#define NODE_H

#include "util.h"
#include <stdbool.h>
#include <time.h>

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
    struct tm* date;
    DateType dateType;

    bool hasCounter;

    bool subTreeIsOpen;

    struct Node* next;
    struct Node* prev;
    struct Node* child;
    struct Node* parent;
} Node;

char* getTypeStr(NodeType);
NodeType getTypeFromString(char*);
bool isFinishedType(NodeType);

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

void moveNodeDown(Node*);
void moveNodeUp(Node*);

void toggleCounter(Node*);
int countAnyTodo(Node*);
int countDoneTodo(Node*);

NodeType cycleNodeType(NodeType);
#endif

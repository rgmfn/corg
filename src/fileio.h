#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>

#include "node.h"

Node* loadFromFile(char*);
void writeToFile(Node*, char*);
#endif

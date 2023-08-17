#ifndef INPUT_H
#define INPUT_H

#include "state.h"

#define ESCAPE '\033'
#define ENTER '\012'
#define BACKSPACE '\177'

void parseInput();
void parseDocumentInput();
void parseTodoInput();
void parseInputInput(char*);

#endif

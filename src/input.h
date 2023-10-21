#ifndef INPUT_H
#define INPUT_H

#include <time.h>

#include "state.h"
#include "util.h"

#define ESCAPE '\033'
#define ENTER '\012'
#define BACKSPACE '\177'

void parseInput();
void parseDocumentInput();
void parseTodoInput();
void parseDateTypeInput();
void parseCalendarInput();
void parseInputInput(char*);

#endif

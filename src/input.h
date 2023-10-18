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
void parseCalendarInput(DateType);
void parseInputInput(char*);

#endif

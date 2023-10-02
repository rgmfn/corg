#ifndef COLORS_H
#define COLORS_H

#include "node.h"

#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define CYAN 5
#define MAGENTA 6
#define WHITE 7
#define GRAY 8
#define BRIGHT_RED 9
#define BRIGHT_GREEN 10
#define BRIGHT_YELLOW 11
#define BRIGHT_BLUE 12
#define BRIGHT_CYAN 14
#define BRIGHT_MAGENTA 13
// yeah, don't know why those two are switched
#define BRIGHT_WHITE 15

#define RED_SEL 16
#define GREEN_SEL 17
#define YELLOW_SEL 18
#define BLUE_SEL 19
#define CYAN_SEL 20
#define MAGENTA_SEL 21
#define WHITE_SEL 22
#define GRAY_SEL 23

int getTypeColor(NodeType);

#endif

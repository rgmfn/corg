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

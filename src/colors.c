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

#include "colors.h"
#include "node.h"

int getTypeColor(NodeType type) {
    switch (type) {
        case Todo:
        case Loop:
        case Idea:
        case Proj:
        case Unchecked:
            return GREEN;
        case Done:
        case Yes:
        case Okay:
        case Checked:
            return GRAY;
        case Wait:
        case Hold:
        case Waiting:
            return YELLOW;
        case Kill:
        case No:
            return RED;
        case Strt:
        case Started:
            return MAGENTA;
        default:
            return 0;
    }
}

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

#ifndef HELP_H
#define HELP_H

#define NUM_HELP_PAGES 4

void goNextHelpPage();
void goPrevHelpPage();

void drawHelpPageOne();
void drawHelpPageTwo();
void drawHelpPageThree();
void drawHelpPageFour();
void drawHelpErrorPage();

#endif

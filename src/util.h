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

#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

#define TODO todo(__LINE__, __FILE__)

#define PAST_DECEMBER -1
#define JANUARY 0
#define FEBRUARY 1
#define MARCH 2
#define APRIL 3
#define MAY 4
#define JUNE 5
#define JULY 6
#define AUGUST 7
#define SEPTEMBER 8
#define OCTOBER 9
#define NOVEMBER 10
#define DECEMBER 11

#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

typedef enum DateType {
    Timestamp,
    Deadline,
    Scheduled,
    Inactive,
    Closed,
} DateType;

void errorAndExit(char*);
void errorAndExitInt(int);
void errorAndExitf(char*, char*);
void todo(int, char*);

char* getMonthFromInt(int);
char* getWeekdayFromInt(int);
int getIntFromWeekday(char*);
struct tm getToday();
struct tm getFirstOfMonth(struct tm);
int getDaysInMonth(int, int);
bool isLeapYear(int);

char* tmToString(struct tm*, DateType);

char getDateTypeOpenChar(DateType);
char getDateTypeClosedChar(DateType);

#endif

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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>

#include "node.h"
#include "util.h"

void errorAndExit(char *msg) {
    endwin();
    printf("ERROR: %s\n", msg);
    exit(EXIT_FAILURE);
}

void errorAndExitInt(int num) {
    endwin();
    printf("ERROR: %d\n", num);
    exit(EXIT_FAILURE);
}

void errorAndExitf(char *msg, char *detail) {
    endwin();
    printf("ERROR: [%s] %s\n", detail, msg);
    exit(EXIT_FAILURE);
}

void todo(int line, char *file) {
    endwin();
    printf("TODO: %s:%d\n", file, line);
    exit(EXIT_FAILURE);
}

/**
 * @param month 0-11 January-December
 * @return full capitalized name of the month
 */
char* getMonthFromInt(int monthNum) {
    switch(monthNum) {
        case JANUARY:
            return "JANUARY";
        case FEBRUARY:
            return "FEBRUARY";
        case MARCH:
            return "MARCH";
        case APRIL:
            return "APRIL";
        case MAY:
            return "MAY";
        case JUNE:
            return "JUNE";
        case JULY:
            return "JULY";
        case AUGUST:
            return "AUGUST";
        case SEPTEMBER:
            return "SEPTEMBER";
        case OCTOBER:
            return "OCTOBER";
        case NOVEMBER:
            return "NOVEMBER";
        case DECEMBER:
            return "DECEMBER";
        default:
            return "ERROR";
    }
}

char* getWeekdayFromInt(int weekdayNum) {
    switch(weekdayNum) {
        case SUNDAY:
            return "SUNDAY";
        case MONDAY:
            return "MONDAY";
        case TUESDAY:
            return "TUESDAY";
        case WEDNESDAY:
            return "WEDNESDAY";
        case THURSDAY:
            return "THURSDAY";
        case FRIDAY:
            return "FRIDAY";
        case SATURDAY:
            return "SATURDAY";
        default:
            return "ERROR";
    }
}

int getIntFromWeekday(char* weekdayStr) {
    const int STRING_SIZE = 3;
    if (strncmp(weekdayStr, "Sun", STRING_SIZE)) {
        return 0;
    } else if (strncmp(weekdayStr, "Mon", STRING_SIZE)) {
        return 1;
    } else if (strncmp(weekdayStr, "Tue", STRING_SIZE)) {
        return 2;
    } else if (strncmp(weekdayStr, "Wed", STRING_SIZE)) {
        return 3;
    } else if (strncmp(weekdayStr, "Thur", STRING_SIZE)) {
        return 4;
    } else if (strncmp(weekdayStr, "Fri", STRING_SIZE)) {
        return 5;
    } else if (strncmp(weekdayStr, "Sat", STRING_SIZE)) {
        return 6;
    } else {
        return -1;
    }
}

char* getShortWeekdayFromInt(int weekdayNum) {
    switch(weekdayNum) {
        case SUNDAY:
            return "Sun";
        case MONDAY:
            return "Mon";
        case TUESDAY:
            return "Tue";
        case WEDNESDAY:
            return "Wed";
        case THURSDAY:
            return "Thu";
        case FRIDAY:
            return "Fri";
        case SATURDAY:
            return "Sat";
        default:
            return "ERROR";
    }
}

struct tm getToday() {
    time_t t = time(NULL);
    return *localtime(&t);
}

struct tm getFirstOfMonth(struct tm day) {
    day.tm_mday %= 7;
    day.tm_wday -= day.tm_mday - 1;
    day.tm_mday -= day.tm_mday - 1;
    day.tm_wday += 7;
    day.tm_wday %= 7;

    return day;
}

int getDaysInMonth(int month, int year) {
    switch (month) {
        case JANUARY:
        case MARCH:
        case MAY:
        case JULY:
        case AUGUST:
        case OCTOBER:
        case DECEMBER:
        case PAST_DECEMBER:
            return 31;
        case APRIL:
        case JUNE:
        case SEPTEMBER:
        case NOVEMBER:
            return 30;
        case FEBRUARY:
            if (isLeapYear(year)) {
                return 29;
            } else {
                return 28;
            }
        default:
            return -1;
    }
}

/**
 * from https://www.programiz.com/c-programming/examples/leap-year
 */
bool isLeapYear(int year) {
    if (year % 400 == 0) {
        return true;
    }
    // not a leap year if divisible by 100
    // but not divisible by 400
    else if (year % 100 == 0) {
        return false;
    }
    // leap year if not divisible by 100
    // but divisible by 4
    else if (year % 4 == 0) {
        return true;
    }

    return false;
}

/**
 * WARNING: MUST FREE STRING AFTER USING
 * TODO; better way to do this?
 */
char* tmToString(struct tm* date, DateType type) {
    char *dateStr = (char*)malloc(17);
    char open = getDateTypeOpenChar(type);
    char closed = getDateTypeClosedChar(type);

    sprintf(dateStr, "%c%d-%02d-%02d %s%c", open, date->tm_year+1900,
            date->tm_mon+1, date->tm_mday,
            getShortWeekdayFromInt(date->tm_wday), closed);

    return dateStr;
}

char getDateTypeOpenChar(DateType dateType) {
    switch (dateType) {
        case Timestamp:
        case Deadline:
        case Scheduled:
            return '<';
            break;
        case Inactive:
        case Closed:
            return '[';
            break;
    }
}

char getDateTypeClosedChar(DateType dateType) {
    switch (dateType) {
        case Timestamp:
        case Deadline:
        case Scheduled:
            return '>';
            break;
        case Inactive:
        case Closed:
            return ']';
            break;
    }
}

void openLink(char *link) {
    char cmd[MAX_LINK_SIZE+10];

#ifdef __unix__
    sprintf(cmd, "xdg-open %s", link);
#elif defined(_WIN32) | defined(_WIN64)
    sprintf(cmd, "start %s", link);
#elif __APPLE__
    sprintf(cmd, "open %s", link);
#endif

    system(cmd);
}

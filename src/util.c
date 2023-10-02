#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>

#include "util.h"

void errorAndExit(char *msg) {
    endwin();
    printf("ERROR: %s\n", msg);
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
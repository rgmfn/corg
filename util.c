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
            break;
        case FEBRUARY:
            return "FEBRUARY";
            break;
        case MARCH:
            return "MARCH";
            break;
        case APRIL:
            return "APRIL";
            break;
        case MAY:
            return "MAY";
            break;
        case JUNE:
            return "JUNE";
            break;
        case JULY:
            return "JULY";
            break;
        case AUGUST:
            return "AUGUST";
            break;
        case SEPTEMBER:
            return "SEPTEMBER";
            break;
        case OCTOBER:
            return "OCTOBER";
            break;
        case NOVEMBER:
            return "NOVEMBER";
            break;
        case DECEMBER:
            return "DECEMBER";
            break;
        default:
            return "ERROR";
            break;
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

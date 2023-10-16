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

char* tmToString(struct tm*);

#endif

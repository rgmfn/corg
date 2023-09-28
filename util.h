#ifndef UTIL_H
#define UTIL_H

#define TODO todo(__LINE__, __FILE__)

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
void errorAndExitf(char*, char*);
void todo(int, char*);

char* getMonthFromInt(int);
struct tm getFirstOfMonth(struct tm);

#endif

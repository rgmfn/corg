#include <time.h>
#include <stdio.h>
#include "../util.h"

void getFirstOfMonthTests() {
    struct tm day, first, expected;

    int inputMDay[] = {2, 19, 29};
    int inputWDay[] = {MONDAY, THURSDAY, WEDNESDAY};
    int inputMon[] = {OCTOBER, OCTOBER, NOVEMBER};
    int expectedWDay[] = {SUNDAY, SUNDAY, WEDNESDAY};

    int numTests = sizeof(inputMDay)/sizeof(int);
    printf("Num Tests: %d\n", numTests);

    for (int testNum = 0; testNum < numTests; testNum++) {
        day.tm_mday = inputMDay[testNum];
        day.tm_wday = inputWDay[testNum];
        day.tm_mon = inputMon[testNum];
        expected.tm_wday = expectedWDay[testNum];
        expected.tm_mon = inputMon[testNum];
        first = getFirstOfMonth(day);

        if (first.tm_wday != expected.tm_wday ||
                first.tm_mday != 1 ||
                first.tm_mon != expected.tm_mon) {
            printf("Test %d Failed:\nExpected %d/%d (%s), got %d/%d (%s)",
                    testNum, expected.tm_mon, expected.tm_mday,
                    getWeekdayFromInt(expected.tm_wday),
                    first.tm_mon, first.tm_mday,
                    getWeekdayFromInt(first.tm_wday));
        } else {
            printf("Test %d Success\n", testNum);
        }
    }
}

int main() {
    getFirstOfMonthTests();

    return 0;
}

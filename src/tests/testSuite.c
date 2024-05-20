#include "utilTests.h"
#include <stdio.h>

int main() {
    int failedTests = 0;

    failedTests += utilTests();

    printf("OVERALL %i Failed Tests\n", failedTests);

    return 0;
}

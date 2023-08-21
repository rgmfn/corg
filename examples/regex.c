#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "../util.h"

/* #define REGEX "([[:alpha:]]+)[[:space:]]([[:alpha:]]+)" */
#define REGEX "^([*]+)[[:blank:]]+(([[:upper:]]{4})[[:blank:]]+)?(([[:alpha:]]+[[:space:]]*)+)$"
#define ERRBUFF_SIZE 100

int main() {
    regex_t regex;
    regmatch_t rm[5];

    char errbuf[ERRBUFF_SIZE];

    char line[1000] = "*** [X] Ian ";

    int compRet = regcomp(&regex, REGEX, REG_EXTENDED);
    if (compRet != 0) {
        regerror(compRet, &regex, errbuf, ERRBUFF_SIZE);
        errorAndExit(errbuf);
    }

    int execRet = regexec(&regex, line, 5, rm, 0);
    if (execRet != 0) {
        regerror(execRet, &regex, errbuf, ERRBUFF_SIZE);
        printf("Error: %s\n", errbuf);
        exit(EXIT_FAILURE);
    }

    printf("<<%.*s>>\n", (int)(rm[0].rm_eo - rm[0].rm_so), line + rm[0].rm_so);
    printf("<<%.*s>>\n", (int)(rm[1].rm_eo - rm[1].rm_so), line + rm[1].rm_so);
    printf("<<%.*s>>\n", (int)(rm[2].rm_eo - rm[2].rm_so), line + rm[2].rm_so);
    printf("<<%.*s>>\n", (int)(rm[3].rm_eo - rm[3].rm_so), line + rm[3].rm_so);
    printf("<<%.*s>>\n", (int)(rm[4].rm_eo - rm[4].rm_so), line + rm[4].rm_so);

    return EXIT_SUCCESS;
}

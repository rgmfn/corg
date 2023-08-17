#include <stdio.h>
#include <string.h>

int main() {
    char string[50];
    strncpy(string, "hello", sizeof(string));
    string[5] = 'b';
    string[3] = '\0';

    printf("%s", string);

    return 0;
}

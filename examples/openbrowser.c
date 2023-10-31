#include <stdlib.h>

int main() {
#ifdef __unix__
    system("xdg-open https://espn.com");
#elif defined(_WIN32) || defined(_WIN64)
    system("start https://espn.com");
#elif __APPLE__
    system("open https://espn.com");
#endif


    return EXIT_SUCCESS;
}

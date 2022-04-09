#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int printf(const char *fmt, ...);

bool is_true(char *test) { return true; }

void funcall(char *name) {
    while (!is_true("test")) {
    }
}

int main() {
    printf("!true = %d\n", !true);
    printf("!false = %d\n", !false);

    if (!false) printf("if (!false) is true\n");
    if (!is_true("test"))
        printf("if (!is_true()) is true\n");
    else
        printf("if (!is_true()) is false\n");

    if (true && !false)
        printf("if (true && !false) is true\n");
    else
        printf("if (true && !false) is false\n");
    `

        while (!is_true("test")) {}

    return 0;
}
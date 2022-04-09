#include <stddef.h>

typedef char *Foo;

void takes_foo(Foo foo) {}

int printf(const char *fmt, ...);

int main() {
    takes_foo(NULL);
    char foo = NULL;

    if (foo == NULL)
        printf("foo == NULL\n");
    else
        printf("foo != NULL\n");

    return 0;
}
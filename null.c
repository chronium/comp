#include <stddef.h>

typedef char *Foo;

void takes_foo(Foo foo);

int main() {
    takes_foo(NULL);
    char foo = NULL;

    if (foo == NULL) {
    }

    return 0;
}
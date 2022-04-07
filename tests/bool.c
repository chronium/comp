#include <stdbool.h>

bool always_true() { return true; }

int main() {
    bool test1 = true;
    bool test2 = false;

    bool testo = test1 && test2;
    bool testoo = test1 || test2;

    bool call = always_true() == true;

    return 0;
}
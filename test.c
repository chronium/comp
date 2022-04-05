void foo();

typedef int bool;

int test(bool adsf);

int test(bool adsf) {
    return 1;
}

void free (void *__ptr);

bool true = 1;
bool false = 0;

char* str_test() {
    return "hello world";
}

int printf(const char *fmt, ...);

int main(int argc, char **argv) {
    test(true);
    printf("Hello World! %s %d %d\n", *argv, true, -1);
    return 0;
}
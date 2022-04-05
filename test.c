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
    if (true) {
        printf("Hello World! 1 + 1 = %d\n", 1 + 1);
    }
    return 0;
}
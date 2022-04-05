void foo();

int test();

int test() {
    return 1;
}

char* str_test() {
    return "hello world";
}

int printf(const char *fmt, ...);

int main(int argc, char **argv) {
    printf("Hello World! %d\n", *argv);
    return 0;
}
typedef struct test {
    int a;
} test_t;

int printf(const char* fmt, ...);

int main() {
    test_t t;

    t.a;
    t.a = 1;

    printf("%d\n", t.a);

    return 0;
}
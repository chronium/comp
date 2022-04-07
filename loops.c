int printf(const char* fmt, ...);

int main() {
    int i = 0;

    while (i < 10) {
        printf("%d\n", i);
        i = i + 1;

        if (i > 5) break;
    }

    return 0;
}
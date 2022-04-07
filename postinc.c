int printf(const char *fmt, ...);

int main() {
    int i = 0;
    printf("%d\n", i++);
    printf("%d\n", i--);
    i++;
    printf("%d\n", i);
    i--;
    printf("%d\n", i);

    return 0;
}
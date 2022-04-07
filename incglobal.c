int printf(const char *fmt, ...);

int _global;

int main() {
    _global = 0;
    printf("%d\n", _global++);
    printf("%d\n", _global);
    return 0;
}
int printf(const char *fmt, ...);

int main(int argc, char **argv) {
    printf("Hello World! %s\n", *argv);
    return 0;
}
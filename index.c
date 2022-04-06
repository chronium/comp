int printf(const char* fmt, ...);

void main(int argc, char** argv) {
    printf("%c\n", "foo"[0]);
    printf("argv[0] = %s\n", argv[0]);
}
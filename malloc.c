void* malloc(int __size);
int printf(const char* fmt, ...);

typedef struct LLVMValue* LLVMValueRef;

int main() {
    LLVMValueRef ref = malloc(64);

    char* ty = "test";

    char* str = malloc(6);
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[6] = '\0';

    printf("%s\n", str);

    return 0;
}
void* malloc(int __size);

typedef struct LLVMValue* LLVMValueRef;

int main() {
    LLVMValueRef ref = malloc(64);
    return 0;
}
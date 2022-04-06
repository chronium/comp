void foo();

typedef int bool;

typedef struct LLVMModule* LLVMModuleRef;
typedef struct LLVMType* LLVMTypeRef;
typedef struct LLVMValue* LLVMValueRef;
typedef struct LLVMBuilder* LLVMBuilderRef;
typedef struct LLVMBasicBlock* LLVMBasicBlockRef;
typedef struct LLVMContext* LLVMContextRef;

LLVMModuleRef LLVMModuleCreateWithName(const char* name);
LLVMValueRef LLVMAddFunction(LLVMModuleRef module, const char* name,
                             LLVMTypeRef type);
LLVMValueRef LLVMGetNamedFunction(LLVMModuleRef module, const char* name);
void LLVMDisposeModule(LLVMModuleRef module);
int LLVMVerifyModule(LLVMModuleRef module, int action, char** output);
LLVMContextRef LLVMGetGlobalContext();
bool LLVMWriteBitcodeToFile(LLVMModuleRef M, const char* Path);

int test(bool adsf);

int test(bool adsf) { return 1; }

void free(void* __ptr);

bool true = 1;
bool false = 0;

char* str_test() { return "hello world"; }

int printf(const char* fmt, ...);

int main(int argc, char** argv) {
    int test = 1234;
    if (true) {
        printf("Hello World! ORDER OF OPERATIONS 1 + 2 * 3 = %d\ntest = %d\n",
               1 + 2 * 3, test);

        char* foobar = "Foobar";
        printf("Foobar[5] = %c\n", foobar[5]);

        printf("argv[0] = %s\n", argv[0]);
    }
    return 0;
}
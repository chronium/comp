#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// #include <llvm-c/Core.h>
// #include <llvm-c/ExecutionEngine.h>
// #include <llvm-c/Target.h>
// #include <llvm-c/Analysis.h>
// #include <llvm-c/BitWriter.h>

typedef int32_t (*funcPtr_t) (int32_t, int32_t);

#define LLVMAbortProcessAction 0
#define LLVMPrintMessageAction 1
#define LLVMReturnStatusAction 2

#define LLVMModuleRef void*
#define LLVMTypeRef void*
#define LLVMValueRef void*
#define LLVMBasicBlockRef void*
#define LLVMBuilderRef void*
#define LLVMExecutionEngineRef void*

LLVMModuleRef LLVMModuleCreateWithName(const char *ModuleID);
LLVMTypeRef LLVMInt32Type(void);
LLVMValueRef LLVMAddFunction(LLVMModuleRef M, const char *Name,
                             LLVMTypeRef FunctionTy);
LLVMBasicBlockRef LLVMAppendBasicBlock(LLVMValueRef Fn, const char *Name);
LLVMTypeRef LLVMFunctionType(LLVMTypeRef ReturnType,
                             LLVMTypeRef *ParamTypes, unsigned ParamCount,
                             int IsVarArg);
LLVMBuilderRef LLVMCreateBuilder(void);
void LLVMPositionBuilderAtEnd(LLVMBuilderRef Builder, LLVMBasicBlockRef Block);
LLVMValueRef LLVMBuildAdd(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                          const char *Name);
LLVMValueRef LLVMGetParam(LLVMValueRef Fn, unsigned Index);
LLVMValueRef LLVMBuildRet(LLVMBuilderRef, LLVMValueRef V);
int LLVMVerifyModule(LLVMModuleRef M, int Action,
                          char **OutMessage);
int LLVMCreateExecutionEngineForModule(LLVMExecutionEngineRef *OutEE,
                                            LLVMModuleRef M,
                                            char **OutError);
void *LLVMGetPointerToGlobal(LLVMExecutionEngineRef EE, LLVMValueRef Global);
int LLVMWriteBitcodeToFile(LLVMModuleRef M, const char *Path);
void LLVMDisposeExecutionEngine(LLVMExecutionEngineRef EE);

void LLVMDisposeMessage(char *Message);
void LLVMDisposeBuilder(LLVMBuilderRef Builder);
void LLVMLinkInMCJIT(void);

void LLVMInitializeX86TargetInfo(void);
void LLVMInitializeX86Target(void);
void LLVMInitializeX86TargetMC(void);
void LLVMInitializeX86AsmPrinter(void);
void LLVMInitializeX86AsmParser(void);

static inline void LLVMInitializeNativeTarget(void) {
  LLVMInitializeX86TargetInfo();
  LLVMInitializeX86Target();
  LLVMInitializeX86TargetMC();
}

static inline void LLVMInitializeNativeAsmPrinter(void) {
  LLVMInitializeX86AsmPrinter();
}

static inline void LLVMInitializeNativeAsmParser(void) {
  LLVMInitializeX86AsmParser();
}

int main(int argc, char const *argv[]) {
    LLVMModuleRef mod = LLVMModuleCreateWithName("my_module");

    LLVMTypeRef param_types[] = { LLVMInt32Type(), LLVMInt32Type() };
    LLVMTypeRef ret_type = LLVMFunctionType(LLVMInt32Type(), param_types, 2, 0);
    LLVMValueRef sum = LLVMAddFunction(mod, "sum", ret_type);

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(sum, "entry");

    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMPositionBuilderAtEnd(builder, entry);
    LLVMValueRef tmp = LLVMBuildAdd(builder, LLVMGetParam(sum, 0), LLVMGetParam(sum, 1), "tmp");
    LLVMBuildRet(builder, tmp);

    char *error = NULL;
    LLVMVerifyModule(mod, LLVMPrintMessageAction, &error);
    LLVMDisposeMessage(error);

    LLVMExecutionEngineRef engine;
    error = NULL;
    LLVMLinkInMCJIT();
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();
    if (LLVMCreateExecutionEngineForModule(&engine, mod, &error) != 0) {
        fprintf(stderr, "failed to create execution engine\n");
        abort();
    }
    if (error) {
        fprintf(stderr, "error: %s\n", error);
        LLVMDisposeMessage(error);
        exit(EXIT_FAILURE);
    }

    if (argc < 3) {
        fprintf(stderr, "usage: %s x y\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int32_t x = strtoll(argv[1], NULL, 10);
    int32_t y = strtoll(argv[2], NULL, 10);

    {
        funcPtr_t funcPtr = (funcPtr_t)LLVMGetPointerToGlobal(engine, sum);
        printf("%d\n", funcPtr(x,y));
    }

    // Write out bitcode to file
    if (LLVMWriteBitcodeToFile(mod, "comp.bc") != 0) {
        fprintf(stderr, "error writing bitcode to file, skipping\n");
    }

    LLVMDisposeBuilder(builder);
    LLVMDisposeExecutionEngine(engine);
    return 0;
}
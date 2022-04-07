#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LLVMModule* LLVMModuleRef;
typedef struct LLVMType* LLVMTypeRef;
typedef struct LLVMValue* LLVMValueRef;
typedef struct LLVMBuilder* LLVMBuilderRef;
typedef struct LLVMBasicBlock* LLVMBasicBlockRef;
typedef struct LLVMContext* LLVMContextRef;

typedef int bool;

int LLVMPrintMessageAction = 1;

typedef int LLVMIntPredicate;

// Module
LLVMModuleRef LLVMModuleCreateWithName(const char* name);
LLVMValueRef LLVMAddFunction(LLVMModuleRef module, const char* name,
                             LLVMTypeRef type);
LLVMValueRef LLVMGetNamedFunction(LLVMModuleRef module, const char* name);
void LLVMDisposeModule(LLVMModuleRef module);
int LLVMVerifyModule(LLVMModuleRef module, int action, char** output);
LLVMContextRef LLVMGetGlobalContext();
bool LLVMWriteBitcodeToFile(LLVMModuleRef M, const char* Path);

// Builder
LLVMBasicBlockRef LLVMAppendBasicBlock(LLVMValueRef location, const char* name);
LLVMBuilderRef LLVMCreateBuilder();
void LLVMPositionBuilderAtEnd(LLVMBuilderRef builder, LLVMBasicBlockRef block);
void LLVMDisposeBuilder(LLVMBuilderRef builder);
LLVMValueRef LLVMBuildRetVoid(LLVMBuilderRef builder);
LLVMValueRef LLVMBuildRet(LLVMBuilderRef builder, LLVMValueRef value);
LLVMValueRef LLVMBuildGlobalString(LLVMBuilderRef builder, const char* Str,
                                   const char* Name);
LLVMValueRef LLVMBuildGlobalStringPtr(LLVMBuilderRef builder, const char* Str,
                                      const char* Name);
LLVMValueRef LLVMBuildCall2(LLVMBuilderRef builder, LLVMTypeRef ret,
                            LLVMValueRef Fn, LLVMValueRef* Args,
                            unsigned NumArgs, const char* Name);
LLVMValueRef LLVMBuildLoad2(LLVMBuilderRef builder, LLVMTypeRef ret,
                            LLVMValueRef Fn, const char* Name);
LLVMValueRef LLVMBuildCondBr(LLVMBuilderRef builder, LLVMValueRef If,
                             LLVMBasicBlockRef Then, LLVMBasicBlockRef Else);
LLVMValueRef LLVMBuildBr(LLVMBuilderRef builder, LLVMBasicBlockRef Dest);
LLVMValueRef LLVMBuildTrunc(LLVMBuilderRef builder, LLVMValueRef val,
                            LLVMTypeRef destty, const char* Name);
LLVMValueRef LLVMBuildAdd(LLVMBuilderRef builder, LLVMValueRef LHS,
                          LLVMValueRef RHS, const char* Name);
LLVMValueRef LLVMBuildSub(LLVMBuilderRef builder, LLVMValueRef LHS,
                          LLVMValueRef RHS, const char* Name);
LLVMValueRef LLVMBuildMul(LLVMBuilderRef builder, LLVMValueRef LHS,
                          LLVMValueRef RHS, const char* Name);
LLVMValueRef LLVMBuildSDiv(LLVMBuilderRef builder, LLVMValueRef LHS,
                           LLVMValueRef RHS, const char* Name);
LLVMValueRef LLVMBuildAlloca(LLVMBuilderRef builder, LLVMTypeRef Ty,
                             const char* Name);
LLVMValueRef LLVMBuildStore(LLVMBuilderRef builder, LLVMValueRef Val,
                            LLVMValueRef Ptr);
LLVMValueRef LLVMBuildGEP2(LLVMBuilderRef B, LLVMTypeRef Ty,
                           LLVMValueRef Pointer, LLVMValueRef* Indices,
                           unsigned NumIndices, const char* Name);
LLVMValueRef LLVMBuildICmp(LLVMBuilderRef, LLVMIntPredicate Op,
                           LLVMValueRef LHS, LLVMValueRef RHS,
                           const char* Name);
LLVMValueRef LLVMBuildNeg(LLVMBuilderRef, LLVMValueRef V, const char* Name);
LLVMValueRef LLVMBuildPointerCast(LLVMBuilderRef, LLVMValueRef Val,
                                  LLVMTypeRef DestTy, const char* Name);
LLVMValueRef LLVMBuildArrayAlloca(LLVMBuilderRef, LLVMTypeRef Ty,
                                  LLVMValueRef Val, const char* Name);
LLVMValueRef LLVMBuildAnd(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                          const char* Name);
LLVMValueRef LLVMBuildOr(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                         const char* Name);
LLVMValueRef LLVMBuildXor(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS,
                          const char* Name);

// Diagnostics
char* LLVMPrintModuleToString(LLVMModuleRef module);
void LLVMDisposeMessage(char* msg);
void LLVMDumpType(LLVMTypeRef Val);

// Types
LLVMTypeRef LLVMInt32Type();
LLVMTypeRef LLVMInt8Type();
LLVMTypeRef LLVMInt1Type();
LLVMTypeRef LLVMVoidType();
LLVMTypeRef LLVMPointerType(LLVMTypeRef ty, unsigned AddressSpace);
LLVMTypeRef LLVMFunctionType(LLVMTypeRef ret, LLVMTypeRef* args,
                             unsigned ParamCount, int IsVarArg);
LLVMTypeRef LLVMTypeOf(LLVMValueRef val);
LLVMTypeRef LLVMGetReturnType(LLVMTypeRef ty);
LLVMTypeRef LLVMGetElementType(LLVMTypeRef ty);
LLVMTypeRef LLVMStructType(LLVMTypeRef* ty, unsigned elemCount, int Packed);
LLVMTypeRef LLVMStructCreateNamed(LLVMContextRef ctx, const char* name);
int LLVMGetTypeKind(LLVMTypeRef ty);

// Values
LLVMValueRef LLVMGetPoison(LLVMTypeRef ty);
LLVMValueRef LLVMConstInt(LLVMTypeRef ty, unsigned long long N, int signExtend);
LLVMValueRef LLVMConstIntOfString(LLVMTypeRef ty, const char* Text, int Radix);
LLVMValueRef LLVMConstString(const char* value, unsigned len,
                             int DontNullTerminate);
LLVMValueRef LLVMGetParam(LLVMValueRef fn, unsigned index);
LLVMValueRef LLVMConstNull(LLVMTypeRef Ty);

// Variables
LLVMValueRef LLVMAddGlobal(LLVMModuleRef module, LLVMTypeRef ty,
                           const char* name);
LLVMValueRef LLVMSetInitializer(LLVMValueRef GlobalVar,
                                LLVMValueRef ConstantVal);
void LLVMSetExternallyInitialized(LLVMValueRef GlobalVar, bool IsExtInit);

LLVMModuleRef _module;
LLVMBuilderRef _builder;
LLVMValueRef _function;

char* _filename;
FILE* _input;

char _curch;

char* _buffer;
int _buflength;
int _token;
int _curln;

int TOKEN_OTHER = 0;
int TOKEN_IDENT = 1;
int TOKEN_INT = 2;
int TOKEN_CHAR = 3;
int TOKEN_STRING = 4;
int TOKEN_VARARGS = 5;
int TOKEN_SYMBOL = 6;

bool true = 1;
bool false = 0;

bool has_returned = 0;

int LLVMArrayTypeKind = 11;
int LLVMPointerTypeKind = 12;

LLVMIntPredicate LLVMIntEQ = 32;   // equal
LLVMIntPredicate LLVMIntNE = 33;   // not equal
LLVMIntPredicate LLVMIntUGT = 34;  // unsigned greater than
LLVMIntPredicate LLVMIntUGE = 35;  // unsigned greater or equal
LLVMIntPredicate LLVMIntULT = 36;  // unsigned less than
LLVMIntPredicate LLVMIntULE = 37;  // unsigned less or equal
LLVMIntPredicate LLVMIntSGT = 38;  // signed greater than
LLVMIntPredicate LLVMIntSGE = 39;  // signed greater or equal
LLVMIntPredicate LLVMIntSLT = 40;  // signed less than
LLVMIntPredicate LLVMIntSLE = 41;  // signed less or equal

char* strdup(const char* str) { return strcpy(malloc(strlen(str) + 1), str); }

void lex_new(char* filename) {
    _filename = strdup(filename);
    _input = fopen(filename, "r");
    _buffer = malloc(256);
    _curln = 1;
}

void lex_drop() {
    free(_buffer);
    free(_filename);
}

void lex_next_char() {
    if (_curch == '\n') _curln++;

    _curch = fgetc(_input);
}

void back_up(char c) { ungetc(c, _input); }

void lex_eat_char() {
    _buffer[_buflength++] = _curch;
    lex_next_char();
}

int errors;

void error(char* format) {
    printf("%s:%d: error: ", _filename, _curln);
    printf(format, _buffer);
    errors++;
}

void errorb(char* format, char* buffer) {
    printf("%s:%d: error: ", _filename, _curln);
    printf(format, buffer);
    errors++;
}

void require(bool condition, char* format) {
    if (!condition) error(format);
}

void lex_eat_escape() {
    if (_curch == '\\') {
        lex_next_char();

        if (_curch == 'n' && !feof(_input))
            _buffer[_buflength++] = '\n';
        else if (_curch == '0' && !feof(_input))
            _buffer[_buflength++] = '\0';
        lex_next_char();
    } else
        lex_eat_char();
}

void lex_next() {
    while (_curch == ' ' || _curch == '\r' || _curch == '\n' || _curch == '\t')
        lex_next_char();

    if (_curch == '#') {
        lex_next_char();

        while (_curch != '\n' && !feof(_input)) lex_next_char();

        lex_next();
        return;
    }

    if (_curch == '/') {
        lex_next_char();

        if (_curch == '/') {
            while (_curch != '\n' && !feof(_input)) lex_next_char();

            lex_next();
            return;
        } else {
            back_up('/');
        }
    }

    _buflength = 0;
    _token = TOKEN_OTHER;

    if (isalpha(_curch) || _curch == '_') {
        _token = TOKEN_IDENT;
        lex_eat_char();

        while ((isalnum(_curch) || _curch == '_') && !feof(_input))
            lex_eat_char();
    } else if (isdigit(_curch) || _curch == '-') {
        _token = TOKEN_INT;
        lex_eat_char();

        while (isdigit(_curch) && !feof(_input)) lex_eat_char();
    } else if (_curch == '\'') {
        lex_next_char();
        if (_curch == '\'') {
            error("Expected at least one character in between \'\'");
        }

        _token = TOKEN_CHAR;

        lex_eat_escape();

        if (_curch != '\'') {
            error("Expected \'");
        }
        lex_next_char();
    } else if (_curch == '"') {
        lex_next_char();
        _token = TOKEN_STRING;

        while (_curch != '"' && !feof(_input)) lex_eat_escape();

        if (_curch != '"') {
            error("Expected \"");
        }
        lex_next_char();
    } else if (_curch == '.') {
        lex_next_char();
        if (_curch != '.') {
            error("Expected .");
        }
        lex_next_char();
        if (_curch != '.') {
            error("Expected .");
        }
        lex_next_char();

        _token = TOKEN_VARARGS;

    } else if (_curch == '+' || _curch == '-' || _curch == '*' ||
               _curch == '/' || _curch == '&' || _curch == '|' ||
               _curch == '=' || _curch == '<' || _curch == '>' ||
               _curch == '!' || _curch == '^' || _curch == '%') {
        lex_eat_char();
        _token = TOKEN_SYMBOL;

        if ((_curch == _buffer[0] && _curch != '!') || _curch == '=')
            lex_eat_char();
    } else
        lex_eat_char();

    _buffer[_buflength++] = '\0';
}

void read() { lex_next(); }

bool peek_tok(char* look) { return !strcmp(_buffer, look); }

void match(char* look) {
    if (!peek_tok(look)) {
        printf("%s:%d: error: expected '%s', found '%s'\n", _filename, _curln,
               look, _buffer);
        errors++;
    }

    read();
}

bool try_match(char* look) {
    if (peek_tok(look)) {
        read();
        return 1;
    }
    return 0;
}

bool waiting_for(char* look) { return !peek_tok(look) & !feof(_input); }

char** locals;
int local_no = 0;
int arg_no = 0;
LLVMValueRef* local_refs;

char** globals;
int global_no = 0;
LLVMValueRef* global_refs;
int* is_fn;

char** typedefs;
int typedef_no = 0;
LLVMTypeRef* typedef_refs;

void sym_init(int max) {
    int size = max * 8;

    locals = malloc(size);
    local_refs = malloc(size);

    globals = malloc(size);
    global_refs = malloc(size);
    is_fn = calloc(max, 8);

    typedefs = malloc(size);
    typedef_refs = malloc(size);
}

void new_typedef(char* ident, LLVMTypeRef type) {
    typedef_refs[typedef_no] = type;
    typedefs[typedef_no++] = ident;
}

void new_global(char* ident, LLVMValueRef ref) {
    global_refs[global_no] = ref;
    globals[global_no++] = ident;
}

void new_fn(char* ident, LLVMValueRef fn) {
    is_fn[global_no] = true;
    new_global(ident, fn);
}

void new_local(char* ident, LLVMValueRef ref) {
    local_refs[local_no] = ref;
    locals[local_no++] = ident;
}

void new_arg(char* ident) {
    new_local(ident, 0);
    arg_no++;
}

void new_scope() {
    local_no = 0;
    arg_no = 0;
}

int sym_lookup(char** table, int table_size, char* look) {
    int i = 0;

    while (i < table_size)
        if (!strncmp(table[i++], look, strlen(look))) return i - 1;

    return -1;
}

int precedence() {
    if (peek_tok("||")) {
        return 10;
    } else if (peek_tok("&&")) {
        return 15;
    } else if (peek_tok("|")) {
        return 20;
    } else if (peek_tok("^")) {
        return 25;
    } else if (peek_tok("&")) {
        return 30;
    } else if (peek_tok("==") || peek_tok("!=")) {
        return 40;
    } else if (peek_tok("<") || peek_tok("<=") || peek_tok(">") ||
               peek_tok(">=")) {
        return 45;
    } else if (peek_tok("+") || peek_tok("-")) {
        return 50;
    } else if (peek_tok("*") || peek_tok("/") || peek_tok("%")) {
        return 60;
    }

    return -1;
}

bool binds_tighter(int right) {
    if (feof(_input)) return false;

    return precedence() > right;
}

LLVMValueRef expr();

LLVMValueRef funcall(char* name) {
    LLVMValueRef argv[8];
    int argc = 0;

    while (!try_match(")")) {
        argv[argc++] = expr(0);

        try_match(",");
    }

    LLVMValueRef func = LLVMGetNamedFunction(_module, name);
    LLVMTypeRef fty = LLVMTypeOf(func);
    LLVMTypeRef retty = LLVMGetReturnType(fty);

    return LLVMBuildCall2(_builder, retty, func, argv, argc, "");
}

int is_indexable(LLVMValueRef value) {
    int kind = LLVMGetTypeKind(LLVMTypeOf(value));

    if (kind == LLVMArrayTypeKind || kind == LLVMPointerTypeKind) return true;

    return false;
}

LLVMValueRef deref_value(LLVMValueRef value) {
    LLVMTypeRef inner = LLVMGetElementType(LLVMTypeOf(value));

    return LLVMBuildLoad2(_builder, inner, value, "");
}

LLVMValueRef atom() {
    LLVMValueRef ret = LLVMGetPoison(LLVMVoidType());
    char* what = strdup(_buffer);

    if (_token == TOKEN_IDENT) {
        char* sym = strdup(_buffer);
        read();

        int global = sym_lookup(globals, global_no, sym);
        int lookup = sym_lookup(locals, local_no, sym);

        if (lookup > -1 && lookup < arg_no) {
            ret = LLVMGetParam(_function, lookup);
        } else if (lookup != -1) {
            LLVMValueRef ref = local_refs[lookup];

            ret = LLVMBuildLoad2(_builder, LLVMGetElementType(LLVMTypeOf(ref)),
                                 ref, "");
        } else if (global != -1) {
            LLVMValueRef ref = global_refs[global];

            if (try_match("(")) {
                ret = funcall(sym);
            } else
                ret = LLVMBuildLoad2(
                    _builder, LLVMGetElementType(LLVMTypeOf(ref)), ref, "");
        } else
            error("Symbol %s not declared");
    } else if (_token == TOKEN_INT) {
        char* val = strdup(_buffer);
        read();

        ret = LLVMConstIntOfString(LLVMInt32Type(), val, 10);
    } else if (_token == TOKEN_CHAR) {
        int val = _buffer[0];
        read();

        ret = LLVMConstInt(LLVMInt8Type(), val, false);
    } else if (_token == TOKEN_STRING) {
        char* val = strdup(_buffer);
        read();
        val[strlen(val)] = '\0';

        ret = LLVMBuildGlobalStringPtr(_builder, val, "");
    } else if (try_match("*")) {
        LLVMValueRef val = expr(0);

        if (is_indexable(val))
            ret = deref_value(val);
        else
            error("Expected derefable value");
    } else if (try_match("!")) {
        LLVMValueRef val = expr(0);

        ret = LLVMBuildNeg(_builder, val, "");
    } else {
        error("expected an atom, found '%s'");

        return ret;
    }

    if (try_match("[")) {
        if (is_indexable(ret)) {
            LLVMValueRef indices[1];
            indices[0] = expr(0);
            LLVMDumpType(LLVMGetElementType(LLVMTypeOf(ret)));
            LLVMTypeRef inner = LLVMGetElementType(LLVMTypeOf(ret));

            ret = LLVMBuildLoad2(
                _builder, inner,
                LLVMBuildGEP2(_builder, inner, ret, indices, 1, ""), "");
        } else
            errorb("%s is not indexable", what);
        match("]");
    }

    return ret;
}

LLVMValueRef rhs(LLVMValueRef left) {
    int prec = precedence();

    if (prec == -1) return left;

    char* op = strdup(_buffer);

    if (!strcmp(op, "+")) {
        read();
        return LLVMBuildAdd(_builder, left, expr(prec), "");
    } else if (!strcmp(op, "-")) {
        read();
        return LLVMBuildSub(_builder, left, expr(prec), "");
    } else if (!strcmp(op, "*")) {
        read();
        return LLVMBuildMul(_builder, left, expr(prec), "");
    } else if (!strcmp(op, "/")) {
        read();
        return LLVMBuildSDiv(_builder, left, expr(prec), "");
    } else if (!strcmp(op, "!=")) {
        read();
        return LLVMBuildICmp(_builder, LLVMIntNE, left, expr(prec), "");
    } else if (!strcmp(op, "<")) {
        read();
        return LLVMBuildICmp(_builder, LLVMIntSLT, left, expr(prec), "");
    } else if (!strcmp(op, "<=")) {
        read();
        return LLVMBuildICmp(_builder, LLVMIntSLE, left, expr(prec), "");
    } else if (!strcmp(op, ">")) {
        read();
        return LLVMBuildICmp(_builder, LLVMIntSGT, left, expr(prec), "");
    } else if (!strcmp(op, ">=")) {
        read();
        return LLVMBuildICmp(_builder, LLVMIntSGE, left, expr(prec), "");
    } else if (!strcmp(op, "==")) {
        read();
        return LLVMBuildICmp(_builder, LLVMIntEQ, left, expr(prec), "");
    } else if (!strcmp(op, "&")) {
        read();
        return LLVMBuildAnd(_builder, left, expr(prec), "");
    } else if (!strcmp(op, "|")) {
        read();
        return LLVMBuildOr(_builder, left, expr(prec), "");
    } else if (!strcmp(op, "^")) {
        read();
        return LLVMBuildXor(_builder, left, expr(prec), "");
    }

    error("Unhandled op %s");
    return LLVMGetPoison(LLVMVoidType());
}

LLVMValueRef expr(int prec) {
    LLVMValueRef left = atom();

    while (binds_tighter(prec)) left = rhs(left);

    return left;
}

void block();

void branch() {
    require(try_match("("), "expected (");
    LLVMValueRef val = expr(0);
    require(try_match(")"), "expected )");

    LLVMBasicBlockRef then = LLVMAppendBasicBlock(_function, "then");
    LLVMBasicBlockRef els = LLVMAppendBasicBlock(_function, "else");

    LLVMBuildCondBr(_builder, LLVMBuildTrunc(_builder, val, LLVMInt1Type(), ""),
                    then, els);

    LLVMPositionBuilderAtEnd(_builder, then);

    block();

    if (!has_returned) LLVMBuildBr(_builder, els);
    LLVMPositionBuilderAtEnd(_builder, els);
}

void while_loop() {
    LLVMBasicBlockRef cond = LLVMAppendBasicBlock(_function, "cond");
    LLVMBasicBlockRef body = LLVMAppendBasicBlock(_function, "body");
    LLVMBasicBlockRef end = LLVMAppendBasicBlock(_function, "end");

    LLVMBuildBr(_builder, cond);

    LLVMPositionBuilderAtEnd(_builder, cond);

    require(try_match("("), "expected (");
    LLVMValueRef cond_val = expr(0);
    require(try_match(")"), "expected )");

    LLVMBuildCondBr(_builder,
                    LLVMBuildTrunc(_builder, cond_val, LLVMInt1Type(), ""),
                    body, end);

    LLVMPositionBuilderAtEnd(_builder, body);

    block();

    if (!has_returned) LLVMBuildBr(_builder, cond);
    LLVMPositionBuilderAtEnd(_builder, end);
}

LLVMTypeRef type(bool suppress) {
    LLVMTypeRef ret = 0;
    bool is_void = false;

    try_match("const");

    if (try_match("int"))
        ret = LLVMInt32Type();
    else if (try_match("void")) {
        ret = LLVMVoidType();
        is_void = true;
    } else if (try_match("char"))
        ret = LLVMInt8Type();
    else if (try_match("struct")) {
        ret = LLVMStructCreateNamed(LLVMGetGlobalContext(), strdup(_buffer));
        read();
    }

    if (ret == 0) {
        int lookup = sym_lookup(typedefs, typedef_no, _buffer);

        if (lookup != -1) {
            ret = typedef_refs[lookup];
            read();
        }

        if (ret == 0) {
            if (!suppress) error("Unknown return type %s\n");
            return LLVMVoidType();
        }
    }

    if (peek_tok("*") && is_void) {
        ret = LLVMInt8Type();
    }

    if (try_match("*"))
        ret = LLVMPointerType(ret, 0);
    else if (try_match("**"))
        ret = LLVMPointerType(LLVMPointerType(ret, 0), 0);

    return ret;
}

void line() {
    if (try_match("return")) {
        has_returned = true;

        if (try_match(";")) {
            LLVMBuildRetVoid(_builder);
            return;
        }

        LLVMBuildRet(_builder, expr(0));

        match(";");
    } else if (try_match("if")) {
        branch();
    } else if (try_match("while")) {
        while_loop();
    } else if (_token == TOKEN_IDENT) {
        char* ident = strdup(_buffer);
        LLVMTypeRef ty = type(true);

        if (_token == TOKEN_IDENT && ty != LLVMVoidType()) {
            char* name = strdup(_buffer);
            read();

            if (try_match("[")) {
                LLVMValueRef count = expr(0);

                match("]");

                LLVMValueRef slot =
                    LLVMBuildAlloca(_builder, LLVMPointerType(ty, 0), name);
                LLVMValueRef ptr =
                    LLVMBuildArrayAlloca(_builder, ty, count, name);

                LLVMBuildStore(_builder, ptr, slot);

                new_local(name, slot);
            } else {
                LLVMValueRef ptr = LLVMBuildAlloca(_builder, ty, name);
                new_local(name, ptr);

                if (try_match("=")) {
                    LLVMValueRef val = expr(0);

                    if (LLVMTypeOf(val) == LLVMPointerType(LLVMInt8Type(), 0) &&
                        LLVMGetTypeKind(ty) == LLVMPointerTypeKind)
                        val = LLVMBuildPointerCast(_builder, val, ty, "");

                    LLVMBuildStore(_builder, val, ptr);
                }
            }
        } else if (_token == TOKEN_IDENT) {
            int global = sym_lookup(globals, global_no, ident);
            int local = sym_lookup(locals, local_no, ident);

            if (local == -1 && global == -1)
                error("Could not find global or local %s\n");
            else {
                read();
                LLVMValueRef ref;
                if (local != -1)
                    ref = local_refs[local];
                else if (global != -1)
                    ref = global_refs[global];

                if (try_match("[")) {
                    if (!is_indexable(ref)) error("lvalue must be indexable");

                    LLVMValueRef indices[1];
                    indices[0] = expr(0);

                    match("]");

                    LLVMTypeRef ty = LLVMGetElementType(LLVMTypeOf(ref));

                    ref = LLVMBuildGEP2(_builder, LLVMGetElementType(ty),
                                        LLVMBuildLoad2(_builder, ty, ref, ""),
                                        indices, 1, "");
                }

                if (try_match("="))
                    LLVMBuildStore(_builder, expr(0), ref);
                else if (try_match("(")) {
                    if (global == -1)
                        errorb("could not find function %s\n", ident);
                    else if (is_fn[global] != true)
                        errorb("%s is not a function\n", ident);
                    else
                        funcall(ident);
                } else
                    error("statement expected");
            }
        }
        match(";");
    }
}

void block() {
    if (try_match("{")) {
        while (waiting_for("}")) line();

        match("}");
    } else
        line();
}

int module_context = 0;
int function_context = 1;

void decl(int context) {
    bool is_typedef = false;
    bool is_impl = false;

    if (try_match("typedef")) is_typedef = true;

    char* typename = strdup(_buffer);

    LLVMTypeRef retty = type(false);

    char* ident = strdup(_buffer);
    read();

    if (try_match("(") && !is_typedef) {
        if (context == module_context) new_scope();

        LLVMTypeRef argv[8];
        int argc = 0;

        bool varargs = false;
        while (!try_match(")")) {
            if (_token == TOKEN_VARARGS) {
                read();
                varargs = true;

                require(try_match(")"),
                        "Varargs must be put at the end of the parameter list");
                break;
            }

            argv[argc++] = type(false);

            new_arg(strdup(_buffer));

            read();
            try_match(",");
        }

        LLVMTypeRef fnty = LLVMFunctionType(retty, argv, argc, varargs);

        if (peek_tok("{")) {
            _function = LLVMGetNamedFunction(_module, ident);

            if (_function == 0) {
                _function = LLVMAddFunction(_module, ident, fnty);
                new_fn(ident, _function);
            }

            int is_main = !strncmp("main", ident, sizeof("main"));

            if (is_main)
                LLVMPositionBuilderAtEnd(
                    _builder, LLVMAppendBasicBlock(_function, "entry"));
            else
                LLVMPositionBuilderAtEnd(_builder,
                                         LLVMAppendBasicBlock(_function, ""));

            is_impl = true;
            block();

            if (!has_returned)
                LLVMBuildRetVoid(_builder);
            else
                has_returned = false;
        } else
            new_fn(ident, LLVMAddFunction(_module, ident, fnty));
    } else if (context == module_context) {
        if (is_typedef) {
            new_typedef(ident, retty);
        } else {
            LLVMValueRef global = LLVMAddGlobal(_module, retty, ident);

            if (try_match("=")) {
                LLVMValueRef val = expr(0);
                LLVMSetInitializer(global, val);
            } else
                LLVMSetInitializer(global, LLVMConstNull(retty));

            new_global(ident, global);
        }
    }

    if (!is_impl) match(";");
}

void gen() {
    while (!feof(_input)) {
        decl(module_context);
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s <file> <out>", argv[0]);
        return 1;
    }

    _module = LLVMModuleCreateWithName(argv[1]);
    _builder = LLVMCreateBuilder();

    lex_new(argv[1]);
    lex_next_char();
    lex_next();

    sym_init(512);

    gen();

    if (errors == 0) {
        char* dump = LLVMPrintModuleToString(_module);
        printf("%s\n", dump);

        char* error = NULL;
        LLVMVerifyModule(_module, LLVMPrintMessageAction, &error);

        if (strlen(error)) {
            // fprintf(stderr, "error: %s\n", error);
            errors += 1;
        }

        if (LLVMWriteBitcodeToFile(_module, argv[2]) != 0) {
            fprintf(stderr, "error writing bitcode to file, skipping\n");
        }
        LLVMDisposeMessage(dump);
        LLVMDisposeMessage(error);
    }

    LLVMDisposeBuilder(_builder);
    LLVMDisposeModule(_module);
    lex_drop();
    return errors != 0;
}
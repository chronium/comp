#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef void* LLVMModuleRef;
typedef void* LLVMTypeRef;
typedef void* LLVMValueRef;
typedef void* LLVMBuilderRef;
typedef void* LLVMBasicBlockRef;
typedef void* LLVMContextRef;

typedef int bool;

#define LLVMPrintMessageAction 1

// Module
LLVMModuleRef LLVMModuleCreateWithName(const char *name);
LLVMValueRef LLVMAddFunction(LLVMModuleRef module, const char *name, LLVMTypeRef type);
LLVMValueRef LLVMGetNamedFunction(LLVMModuleRef module, const char* name);
void LLVMDisposeModule(LLVMModuleRef module);
int LLVMVerifyModule(LLVMModuleRef module, int action, char **output);
LLVMContextRef LLVMGetGlobalContext();

// Builder
LLVMBasicBlockRef LLVMAppendBasicBlock(LLVMValueRef location , const char *name);
LLVMBuilderRef LLVMCreateBuilder();
void LLVMPositionBuilderAtEnd(LLVMBuilderRef builder, LLVMBasicBlockRef block);
void LLVMDisposeBuilder(LLVMBuilderRef builder);
LLVMValueRef LLVMBuildRetVoid(LLVMBuilderRef builder);
LLVMValueRef LLVMBuildRet(LLVMBuilderRef builder, LLVMValueRef value);
LLVMValueRef LLVMBuildGlobalString(LLVMBuilderRef builder, const char* Str, const char* Name);
LLVMValueRef LLVMBuildGlobalStringPtr(LLVMBuilderRef builder, const char* Str, const char* Name);
LLVMValueRef LLVMBuildCall2(LLVMBuilderRef builder, LLVMTypeRef ret, LLVMValueRef Fn, LLVMValueRef *Args, unsigned NumArgs, const char* Name);
LLVMValueRef LLVMBuildLoad2(LLVMBuilderRef builder, LLVMTypeRef ret, LLVMValueRef Fn, const char* Name);

// Diagnostics
char* LLVMPrintModuleToString(LLVMModuleRef module);
void LLVMDisposeMessage(char *msg);
void LLVMDumpType(LLVMTypeRef Val);

// Types
LLVMTypeRef LLVMInt32Type();
LLVMTypeRef LLVMInt8Type();
LLVMTypeRef LLVMVoidType();
LLVMTypeRef LLVMPointerType(LLVMTypeRef ty, unsigned AddressSpace);
LLVMTypeRef LLVMFunctionType(LLVMTypeRef ret, LLVMTypeRef *args, unsigned ParamCount, int IsVarArg);
LLVMTypeRef LLVMTypeOf(LLVMValueRef val);
LLVMTypeRef LLVMGetReturnType(LLVMTypeRef ty);
LLVMTypeRef LLVMGetElementType(LLVMTypeRef ty);
LLVMTypeRef LLVMStructType(LLVMTypeRef* ty, unsigned elemCount, int Packed);
LLVMTypeRef LLVMStructCreateNamed(LLVMContextRef ctx, const char* name);
int LLVMGetTypeKind(LLVMTypeRef ty);

// Values
LLVMValueRef LLVMGetPoison(LLVMTypeRef ty);
LLVMValueRef LLVMConstInt(LLVMTypeRef ty, unsigned long long N, int signExtend);
LLVMValueRef LLVMConstIntOfString(LLVMTypeRef ty, const char *Text, int Radix);
LLVMValueRef LLVMConstString (const char *value, unsigned len, int DontNullTerminate);
LLVMValueRef LLVMGetParam(LLVMValueRef fn, unsigned index);

// Variables
LLVMValueRef LLVMAddGlobal(LLVMModuleRef module, LLVMTypeRef ty, const char* name);
LLVMValueRef LLVMSetInitializer(LLVMValueRef GlobalVar, LLVMValueRef ConstantVal);

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
int TOKEN_STAR = 6;

bool true = 1;
bool false = 0;

bool has_returned = 0;

int LLVMArrayTypeKind = 11;
int LLVMPointerTypeKind = 12;

char* strdup (const char* str) {
    return strcpy(malloc(strlen(str)+1), str);
}

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
    if (_curch == '\n')
        _curln++;

    _curch = fgetc(_input);
}

void back_up(char c) {
    ungetc(c, _input);
}

void lex_eat_char() {
    _buffer[_buflength++] = _curch;
    lex_next_char();
}

int errors;

void error (char* format) {
    printf("%s:%d: error: ", _filename, _curln);
    printf(format, _buffer);
    errors++;
}

void require (bool condition, char* format) {
    if (!condition)
        error(format);
}

void lex_eat_escape() {
    if (_curch == '\\') {
        lex_next_char();

        if (_curch == 'n' && !feof(_input))
            _buffer[_buflength++] = '\n';
        lex_next_char();
    } else
        lex_eat_char();
}

void lex_next() {
    while (_curch == ' ' || _curch == '\r' || _curch == '\n' || _curch == '\t')
        lex_next_char();
    
    if (_curch == '#') {
        lex_next_char();

        while (_curch != '\n' && !feof(_input))
            lex_next_char();
        
        lex_next();
        return;
    }

    if (_curch == '/') {
        lex_next_char();

        if (_curch == '/') {
            while (_curch != '\n' && !feof(_input))
                lex_next_char();
            
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

        while (isdigit(_curch) && !feof(_input))
            lex_eat_char();
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

        while (_curch != '"' && !feof(_input))
            lex_eat_escape();
        
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

    } else if(_curch == '*') {
        lex_eat_char();
        _token = TOKEN_STAR;
    } else
        lex_eat_char();

    _buffer[_buflength++] = '\0';
}

void read() {
    lex_next();
}

bool peek_tok(char* look) {
    return !strcmp(_buffer, look);
}

void match(char* look) {
    if (!peek_tok(look)) {
        printf("%s:%d: error: expected '%s', found '%s'\n", _filename, _curln, look, _buffer);
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

bool waiting_for(char* look) {
    return !peek_tok(look) & !feof(_input);
}

char** locals;
int local_no = 0;
int arg_no = 0;

char** globals;
int global_no = 0;
LLVMValueRef *global_refs;
int *is_fn;

char** typedefs;
int typedef_no = 0;
LLVMTypeRef *typedef_refs;

void sym_init(int max) {
    int size = max * 8;

    locals = malloc(size);

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

void new_local(char* ident) {
    locals[local_no++] = ident;
}

void new_arg(char* ident) {
    new_local(ident);
    arg_no++;
}

void new_scope() {
    local_no = 0;
    arg_no = 0;
}

int sym_lookup(char** table, int table_size, char* look) {
    int i = 0;

    while (i < table_size)
        if (!strncmp(table[i++], look, strlen(look)))
            return i-1;

    return -1;
}

LLVMValueRef expr();

LLVMValueRef funcall(char* name) {
    LLVMValueRef *argv = malloc(64);
    int argc = 0;

    while (!try_match(")")) {
        argv[argc++] = expr();

        try_match(",");
    }

    LLVMValueRef func = LLVMGetNamedFunction(_module, name);
    LLVMTypeRef fty = LLVMTypeOf(func);
    LLVMTypeRef retty = LLVMGetReturnType(fty);

    return LLVMBuildCall2(_builder, retty, func, argv, argc, "");
}

int is_indexable(LLVMValueRef value) {
    int kind = LLVMGetTypeKind(LLVMTypeOf(value));

    if (kind == LLVMArrayTypeKind || kind == LLVMPointerTypeKind)
        return true;

    return false;
}

LLVMValueRef deref_value(LLVMValueRef value) {
    LLVMTypeRef inner = LLVMGetElementType(LLVMTypeOf(value));

    return LLVMBuildLoad2(_builder, inner, value, "");
}

LLVMValueRef atom() {
    if (_token == TOKEN_IDENT) {
        char* sym = strdup(_buffer);
        read();

        int global = sym_lookup(globals, global_no, sym);
        int lookup = sym_lookup(locals, local_no, sym);

        if (lookup > -1 && lookup < arg_no) {
            return LLVMGetParam(_function, lookup);
        }
        
        if (lookup != -1) {
            error("unimplemented");
        } else if (global != -1) {
            LLVMValueRef ref = global_refs[global];

            return LLVMBuildLoad2(_builder, LLVMGetElementType(LLVMTypeOf(ref)), ref, "");
        } else
            error("Symbol %s not declared");
    } else if (_token == TOKEN_INT) {
        char* val = strdup(_buffer);
        read();

        return LLVMConstIntOfString(LLVMInt32Type(), val, 10);
    } else if (_token == TOKEN_CHAR) {
        int val = _buffer[0];
        read();

        return LLVMConstInt(LLVMInt8Type(), val, false);
    } else if (_token == TOKEN_STRING) {
        char* val = strdup(_buffer);
        read();
        val[strlen(val)] = '\0';

        return LLVMBuildGlobalStringPtr(_builder, val, "");
    } else if (_token == TOKEN_STAR) {
        read();

        LLVMValueRef val = expr();

        if (is_indexable(val))
            return deref_value(val);
        else
            error("Expected derefable value");
    } else {
        error("expected an atom, found '%s'");
    }
    return LLVMGetPoison(LLVMVoidType());
}

LLVMValueRef expr() {
    return atom();
}

void line() {
    if (try_match("return")) {
        has_returned = true;

        if (try_match(";")) {
            LLVMBuildRetVoid(_builder);
            return;
        }

        LLVMBuildRet(_builder, expr());
    } else if (_token == TOKEN_IDENT) {
        char* name = strdup(_buffer);
        read();

        require(try_match("("), "Expected ( at start of argument list");

        funcall(name);
    }
    
    match(";");
}

void block() {
    if (try_match("{")) {
        while(waiting_for("}"))
            line();
        
        match("}");
    } else
        line();
}

LLVMTypeRef type() {
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

    if (ret == 0) {
        int lookup = sym_lookup(typedefs, typedef_no, _buffer);

        if (lookup != -1) {
            ret = typedef_refs[lookup];
            read();
        }

        if (ret == 0) {
            error("Unknown return type %s\n");
            return LLVMVoidType();
        }
    }

    if (peek_tok("*") && is_void)
        ret = LLVMInt8Type();

    while (try_match("*"))
        ret = LLVMPointerType(ret, 0);

    return ret;
}

int module_context = 0;
int function_context = 1;

void decl(int context) {
    bool is_typedef = false;
    bool is_impl = false;

    if (try_match("typedef"))
        is_typedef = true;

    char* typename = strdup(_buffer);

    LLVMTypeRef retty = type();

    char* ident = strdup(_buffer);
    read();

    if (try_match("(") && !is_typedef) {
        if (context == module_context)
            new_scope();

        LLVMTypeRef *argv = malloc(64);
        int argc = 0;

        bool varargs = false;
        while (!try_match(")")) {
            if (_token == TOKEN_VARARGS) {
                read();
                varargs = true;

                require(try_match(")"), "Varargs must be put at the end of the parameter list");
                break;
            }

            argv[argc++] = type();

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
                LLVMPositionBuilderAtEnd(_builder, LLVMAppendBasicBlock(_function, "entry"));
            else
                LLVMPositionBuilderAtEnd(_builder, LLVMAppendBasicBlock(_function, ""));

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
                LLVMValueRef val = expr();
                LLVMSetInitializer(global, val);
            }

            new_global(ident, global);
        }
    }

    if (!is_impl)
        match(";");
}

void gen() {
    while(!feof(_input)) {
        decl(module_context);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <file>", argv[0]);
        return 1;
    }

    _module = LLVMModuleCreateWithName(argv[1]);
    _builder = LLVMCreateBuilder();

    lex_new(argv[1]);
    lex_next_char();
    lex_next();
    
    sym_init(512);

    gen();

    char* dump = LLVMPrintModuleToString(_module);
    printf("%s\n", dump);

    char *error = NULL;
    LLVMVerifyModule(_module, LLVMPrintMessageAction, &error);

    if (strlen(error)) {
        //fprintf(stderr, "error: %s\n", error);
        errors += 1;
    }

    LLVMDisposeMessage(error);
    LLVMDisposeMessage(dump);
    LLVMDisposeBuilder(_builder);
    LLVMDisposeModule(_module);
    lex_drop();
    return errors != 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LLVMModuleRef void*
#define LLVMTypeRef void*
#define LLVMValueRef void*
#define LLVMBuilderRef void*
#define LLVMBasicBlockRef void*

#define LLVMPrintMessageAction 1

// Module
LLVMModuleRef LLVMModuleCreateWithName(const char *);
LLVMValueRef LLVMAddFunction(LLVMModuleRef, const char *, LLVMTypeRef);
LLVMValueRef LLVMGetNamedFunction(LLVMModuleRef, const char*);
void LLVMDisposeModule(LLVMModuleRef);
int LLVMVerifyModule(LLVMModuleRef, int, char **);

// Builder
LLVMBasicBlockRef LLVMAppendBasicBlock(LLVMValueRef, const char *);
LLVMBuilderRef LLVMCreateBuilder();
void LLVMPositionBuilderAtEnd(LLVMBuilderRef, LLVMBasicBlockRef);
void LLVMDisposeBuilder(LLVMBuilderRef);
LLVMValueRef LLVMBuildRetVoid(LLVMBuilderRef);
LLVMValueRef LLVMBuildRet(LLVMBuilderRef, LLVMValueRef);
LLVMValueRef LLVMBuildGlobalString(LLVMBuilderRef, const char* Str, const char* Name);
LLVMValueRef LLVMBuildGlobalStringPtr(LLVMBuilderRef, const char* Str, const char* Name);
LLVMValueRef LLVMBuildCall2(LLVMBuilderRef, LLVMTypeRef, LLVMValueRef Fn, LLVMValueRef *Args, unsigned NumArgs, const char* Name);
LLVMValueRef LLVMBuildLoad2(LLVMBuilderRef, LLVMTypeRef, LLVMValueRef, const char*);

// Diagnostics
char* LLVMPrintModuleToString(LLVMModuleRef);
void LLVMDisposeMessage(char *);
void LLVMDumpType(LLVMTypeRef Val);

// Types
LLVMTypeRef LLVMInt32Type(void);
LLVMTypeRef LLVMInt8Type(void);
LLVMTypeRef LLVMVoidType(void);
LLVMTypeRef LLVMPointerType(LLVMTypeRef, unsigned AddressSpace);
LLVMTypeRef LLVMFunctionType(LLVMTypeRef, LLVMTypeRef *, unsigned ParamCount, int IsVarArg);
LLVMTypeRef LLVMTypeOf(LLVMValueRef);
LLVMTypeRef LLVMGetReturnType(LLVMTypeRef);
LLVMTypeRef LLVMGetElementType(LLVMTypeRef);
int LLVMGetTypeKind(LLVMTypeRef);

// Values
LLVMValueRef LLVMGetPoison(LLVMTypeRef);
LLVMValueRef LLVMConstInt(LLVMTypeRef, unsigned long long N, int signExtend);
LLVMValueRef LLVMConstIntOfString(LLVMTypeRef, const char *Text, int Radix);
LLVMValueRef LLVMConstString (const char *, unsigned, int DontNullTerminate);
LLVMValueRef LLVMGetParam(LLVMValueRef, unsigned);

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

int true = 1;
int false = 0;

int has_returned = 0;

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

void require (int condition, char* format) {
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

    if (isalpha(_curch)) {
        _token = TOKEN_IDENT;
        lex_eat_char();

        while ((isalnum(_curch) || _curch == '_') && !feof(_input))
            lex_eat_char();
    } else if (isdigit(_curch)) {
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

int peek_tok(char* look) {
    return !strcmp(_buffer, look);
}

void match(char* look) {
    if (!peek_tok(look)) {
        printf("%s:%d: error: expected '%s', found '%s'\n", _filename, _curln, look, _buffer);
        errors++;
    }

    read();
}

int try_match(char* look) {
    if (peek_tok(look)) {
        read();
        return 1;
    }
    return 0;
}

int waiting_for(char* look) {
    return !peek_tok(look) & !feof(_input);
}

char** locals;
int local_no = 0;
int arg_no = 0;

void sym_init(int max) {
    locals = malloc(max * 8);
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
        if (!strcmp(table[i++], look))
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

        int lookup =  sym_lookup(locals, local_no, sym);

        if (lookup < arg_no) {
            return LLVMGetParam(_function, lookup);
        }

        error("unhandled ident atom");
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

    try_match("const");

    if (try_match("int")) {
        ret = LLVMInt32Type();
    } else if (try_match("void")) {
        ret = LLVMVoidType();
    } else if (try_match("char")) {
        ret = LLVMInt8Type();
    }

    if (ret == 0) {
        error("Unknown return type %s\n");
        return LLVMVoidType();
    }

    while (try_match("*"))
        ret = LLVMPointerType(ret, 0);

    return ret;
}

int module_context = 0;
int function_context = 1;

void decl(int context) {
    int is_impl = false;

    char* typename = strdup(_buffer);

    LLVMTypeRef retty = type();

    char* ident = strdup(_buffer);
    read();

    if (try_match("(")) {
        if (context == module_context)
            new_scope();

        LLVMTypeRef *argv = malloc(64);
        int argc = 0;

        int varargs = false;
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

            if (_function == 0)
                _function = LLVMAddFunction(_module, ident, fnty);

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
        } else {
            LLVMAddFunction(_module, ident, fnty);
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
        fprintf(stderr, "error: %s\n", error);
        errors += 1;
    }

    LLVMDisposeMessage(error);
    LLVMDisposeMessage(dump);
    LLVMDisposeBuilder(_builder);
    LLVMDisposeModule(_module);
    lex_drop();
    return errors != 0;
}
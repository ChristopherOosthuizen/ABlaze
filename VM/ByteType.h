

#ifndef ABLAZE_BYTETYPE_H
#define ABLAZE_BYTETYPE_H

enum class ByteType{
        PUSH,ADD,DIVIDE,TIMES,MINUS,MOD,INT,HALT,DOUBLE,CHAR,STRING,POW,BOOL,SQRT,
        LOAD,STORE,ASI,EQUAL,TRUE,FALSE,
        LABEL,IDEN,CALL,RETURN,PRINT,PRINTLN,
        XOR,NOT,AND,AND_AND,OR,OR_OR,ISGT,ISLT,ISLE,ISGE,
        JMP,JIF,
        CREATELOCAL,POPLOCAL,NEW,APPEND,DELETE, AT,
        LIST,OBJ,STRUCT,STRUCTCONST,SET,SELECT,NIL,INPUT,CAST,WRITEFILE,READFILE,EXISTS,CREATEFILE,DELETEFILE,LEN,LOADCLASS,CLASSCALL,STRUCTCONSTEX,FUNCTIONPUSH,
        SINE,COS,TAN,LSHIFT,RSHIFT,NEG,POP,DUP,MAP,
        
};
#endif

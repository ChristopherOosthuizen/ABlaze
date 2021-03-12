

#ifndef ABLAZE_BYTETYPE_H
#define ABLAZE_BYTETYPE_H

enum class ByteType{
        PUSH,ADD,DIVIDE,TIMES,MINUS,INT,HALT,DOUBLE,CHAR,STRING,POW,SQRT,
        LOAD,STORE,ASI,EQUAL,
        LABEL,IDEN,CALL,RETURN,PRINT,PRINTLN,
        XOR,NOT,AND,OR,ISGT,ISLT,ISLE,ISGE,
        JMP,JIF,
        CREATELOCAL,POPLOCAL,NEW,APPEND,DELETE, AT,
        LIST,OBJ,STRUCT,STRUCTCONST,SET,SELECT,NIL,INPUT,CAST,WRITEFILE,READFILE,EXISTS,CREATEFILE,DELETEFILE,LEN,LOADCLASS,
        
};
#endif

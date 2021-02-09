
#ifndef ABLAZE_BYTEGEN_H
#define ABLAZE_BYTEGEN_H
#include <string>
#include "AST.h"
#include <vector>
#include "Lexer.h"
using namespace std;

class ByteGen{
        Body* m_ast;
        vector<string>* m_lines;
        void expressionToByte(Expression* expr);
        void toCommand(const string& com, const string& parm1, const string& parm2);
        void binToCommand(BinOP* op);
        void decToCommand(Decleration* op);
        void functionCallToByte(FunctionCall* call);
        public:
                ByteGen(Body* ast);
                vector<string>* generateByteCode();
                
};
#endif

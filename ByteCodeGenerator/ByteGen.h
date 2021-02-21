
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
        void toCommand(const string& com);
        void binToCommand(BinOP* op);
        void decToCommand(Decleration* op);
        void functionCallToByte(FunctionCall* call);
        void bodyToByte(Body* body);
        void whileToByte(Body* body,string line);
        void forToByte(Body* body,string line);
        void functionToByte(Body* body);
        void unToByte(Unary* unary);
        void returnToByte(Return* ret);
        public:
                ByteGen(Body* ast);
                vector<string>* generateByteCode();
                
};
#endif

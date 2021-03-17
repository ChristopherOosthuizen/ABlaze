
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
        void argsToBye(vector<Expression*>* args);
        void expressionToByte(Expression* expr);
        void toCommand(const string& com);
        void binToCommand(BinOP* op);
        void decToCommand(Decleration* op,bool isfunc);
        void functionCallToByte(FunctionCall* call);
        void bodyToByte(Body* body);
        void whileToByte(Body* body,string line);
        void forToByte(Body* body,string line);
        void functionToByte(Body* body);
        void unToByte(Unary* unary);
        void structToByte(Body* body);
        void structDec(Body* body);
        void classFunc(string name,Body* body);
        void dotToByte(Dot* dot);
        void arrayToByte(ArrayLiteral* literal);
        void builtInToByte(BuiltIn* builtin);
        void castToByte(Cast* cast);
        bool isBuiltIn(string name);
        void array(Array* arr);
        public:
                ByteGen(Body* ast);
                vector<string>* generateByteCode();
                
};
#endif

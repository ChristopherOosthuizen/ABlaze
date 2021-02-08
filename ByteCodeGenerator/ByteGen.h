
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
        string toCommand(const string& com, const string& parm1, const string& parm2);
        public:
                ByteGen(Body* ast);
                vector<string>* generateByteCode();
                
};
#endif

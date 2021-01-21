/* 
 * Created by Christopher Oosthuizen on 08/01/2021.
 * This files main purpose is to hold the structes defined
 * by context free grammer to create a abstract syntax tree
 *
 */

#ifndef ABLAZE_AST_H
#define ABLAZE_AST_H
#include "Token.h"
#include <string>
#include <vector>

class Expression{
public:
    virtual string name(){return "Expression";}
};

class Literal: public Expression{
    public:
        string name() override;
        Literal(Token* token);
        Token* m_token;
};

    
    
class BinOP: public Expression{
    public:
        string name() override;
        BinOP(Expression* left, Token* op, Expression* right);
        Token* m_op;
        Expression* m_left;
        Expression* m_right;

};

class Decleration: public Expression{

    public:
        string name() override;
        Decleration(Literal* type, Literal* name,Expression* value, bool initalize);
        Literal* m_name;
        Expression* m_value;
        Literal* m_type;
        bool m_initalize;
};

class Body: public Expression{
        public:
                string name() override;
                Body(Expression* control, vector<Expression*>* expressions);
                Expression* m_control;
                vector<Expression*>* m_lines;

};

class FunctionCall: public Expression{
        public:
                string name() override;
                FunctionCall(Literal* name,  vector<Expression*>* args);
                Literal* m_name;
                vector<Expression*>* m_args;
};

class Unary: public Expression{
        public:
                string name() override;
                Unary(Literal* op, Literal* iden);
                Literal* m_op;
                Literal* m_iden;
};

#endif

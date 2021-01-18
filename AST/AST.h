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

    

class Unary: public Expression{
    public:
        string name() override;
        Unary(Token* unaryEpression, Token* expression);
        Token* m_unaryEpression;
        Token* m_expression;
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
        Decleration(Literal* name,Expression* value, bool initalize);
        Literal* m_name;
        Expression* m_value;
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
                FunctionCall(string name,  vector<Expression*>* args);
                string m_name;
                vector<Expression*>* m_args;
};

#endif

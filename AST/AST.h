/* 
 * Created by Christopher Oosthuizen on 08/01/2021.
 * This files main purpose is to hold the structes defined
 * by context free grammer to create a abstract syntax tree
 *
 */

#ifndef ABLAZE_AST_H
#define ABLAZE_AST_H
#include "Token.h"


class Expression{

};

class Literal: public Expression{
    public:
        Literal(Token* token);
        Token* m_token;
};

    

class Unary: public Expression{
    public:
        Unary(Token* unaryEpression, Token* expression);
        Token* m_unaryEpression;
        Token* m_expression;
};

    
class BinOP: public Expression{
    public:
        BinOP(Expression* left, Token* op, Expression* right);
        Token* m_op;
        Expression* m_left;
        Expression* m_right;

};


#endif

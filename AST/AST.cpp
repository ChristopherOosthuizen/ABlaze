#include "AST.h"


Literal::Literal(Token* token){
    m_token = token;
}
Unary::Unary(Token* unaryEpression,Token* expression){
   m_unaryEpression = unaryEpression;
   m_expression = expression;
}

BinOP::BinOP(Expression* left, Token* op, Expression* right){
    m_op = op;
    m_left = left;
    m_right = right;
}

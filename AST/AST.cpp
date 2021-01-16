#include "AST.h"


Literal::Literal(Token* token){
    m_token = token;
}
string Literal::name(){return "Literal";}

Unary::Unary(Token* unaryEpression,Token* expression){
   m_unaryEpression = unaryEpression;
   m_expression = expression;
}
string Unary::name(){return "Unary";}

BinOP::BinOP(Expression* left, Token* op, Expression* right){
    m_op = op;
    m_left = left;
    m_right = right;
}
string BinOP::name(){return "BinOP";}

Decleration::Decleration(Literal* name , Expression* value, bool initalize){
    m_name = name; 
    m_value = value;
    m_initalize= initalize;
}
string Decleration::name(){return "Decleration";}


Body::Body(Expression* control, vector<Expression*>* expressions){
        m_control = control;
        m_lines = expressions;
        
}

string Body::name(){return "Body";}

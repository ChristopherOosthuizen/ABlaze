#include "AST.h"


Literal::Literal(Token* token){
    m_token = token;
}
string Literal::name(){return "Literal";}

BinOP::BinOP(Expression* left, Token* op, Expression* right){
    m_op = op;
    m_left = left;
    m_right = right;
}
string BinOP::name(){return "BinOP";}

Decleration::Decleration(Literal* type, Literal* name , Expression* value, bool initalize){
    m_name = name; 
    m_value = value;
    m_type = type;
    m_initalize= initalize;
}
string Decleration::name(){return "Decleration";}


Body::Body(Expression* control, vector<Expression*>* expressions){
        m_control = control;
        m_lines = expressions;
        
}

string Body::name(){return "Body";}

FunctionCall::FunctionCall(Literal* name, vector<Expression*>* args){
        m_name= name;
        m_args = args;
}

string FunctionCall::name(){ return "FunctionCall";}

Unary::Unary(Literal* op, Literal* iden){
        m_op = op;
        m_iden = iden;
}

string Unary::name(){ return "UnOP";}

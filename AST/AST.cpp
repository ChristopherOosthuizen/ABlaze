#include "AST.h"


Literal::Literal(Token* token){
    m_token = token;
}
string Literal::name(){return "Literal";}

Literal::~Literal(){
        delete m_token;
}



BinOP::BinOP(Expression* left, Token* op, Expression* right){
    m_op = op;
    m_left = left;
    m_right = right;
}
string BinOP::name(){return "BinOP";}

BinOP::~BinOP(){
        delete m_left;
        delete m_right;
        delete m_op;
}

Decleration::Decleration(Literal* type, Expression* name ,Literal* op, Expression* value, bool initalize, bool isArray){
    m_name = name; 
    m_op = op;
    m_value = value;
    m_type = type;
    m_initalize= initalize;
    m_isArray = isArray;
}
string Decleration::name(){return "Decleration";}

Decleration::~Decleration(){
        delete m_op;
        delete m_value;
        delete m_type;
        delete m_name;
}


Body::Body(Expression* control, vector<Expression*>* expressions){
        m_control = control;
        m_lines = expressions;
        
}

string Body::name(){return "Body";}

Body::~Body(){
       delete m_control;
       for(int i=0; i< m_lines->size(); i++){
               delete m_lines->at(i); 
       }
       delete m_lines;
        
}
FunctionCall::FunctionCall(Literal* name, vector<Expression*>* args){
        m_name= name;
        m_args = args;
}

string FunctionCall::name(){ return "FunctionCall";}

FunctionCall::~FunctionCall(){
        delete m_name;
       for(int i=0; i< m_args->size(); i++){
               delete m_args->at(i); 
       }
         
       delete m_args;
}
Unary::Unary(Literal* op, Literal* iden, bool post){
        m_op = op;
        m_iden = iden;
        m_post = post;
}

string Unary::name(){ return "UnOP";}

Unary::~Unary(){
        delete m_op;
        delete m_iden;
}

IfStat::IfStat(Expression* control){
        m_control = control;
}

string IfStat::name(){ return "If";}
string ElseStat::name(){ return "Else";}


IfStat::~IfStat(){
        delete m_control;        
}

WhileStat::WhileStat(Expression* control){
        m_control = control;
}

string WhileStat::name(){ return "While";}

WhileStat::~WhileStat(){
        delete m_control;        
}
ForStat::ForStat(Expression* initial, Expression* condition, Expression* repitition){
        m_initial = initial;
        m_condition = condition;
        m_repitition = repitition;
}

string ForStat::name(){ return "For";}
ForStat::~ForStat(){
       delete m_initial;
       delete m_condition;
       delete m_repitition;
}
Function::Function(Literal* type,FunctionCall* call){
        m_type = type;
        m_call = call;

}

string Function::name(){ return "Function";}

Function::~Function(){
       delete m_type;
       delete m_call;
}
Return::Return(Expression* value){
        m_value = value;
}

string Return::name(){ return "Return";}

Return::~Return(){
        delete m_value;        
}

Import::Import(Expression* value){
        m_value = value;
}

string Import::name(){ return "Import";}

Import::~Import(){
        delete m_value;        
}

ArrayLiteral::ArrayLiteral(Literal* iden, Expression* value){
        m_value = value;
        m_iden = iden;
}

string ArrayLiteral::name(){ return "ArrayLiteral";};

ArrayLiteral::~ArrayLiteral(){
        delete m_value;
        delete m_iden;
}

string Struct::name(){return "Struct";}

Struct::Struct(Literal* iden){
        m_iden = iden;
}

Struct::~Struct(){
        delete m_iden;
}



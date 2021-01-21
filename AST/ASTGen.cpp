// Created by Christopher Oosthuizen on 01/11/2021
// The AST generator is meant to return 
// A based off a list of tokens constructed by a lexer
#include "ASTGen.h"
#include "AST.h"


ASTGen::ASTGen(vector<Token*> tokens){
    m_tokens = tokens;
    m_pos = 0;
}

// return a AST based on the tokens given in
// the construction
Body* ASTGen::generateAST(){
        vector<Expression*>* lines = new vector<Expression*>;
        while(peek() != NULL){
                lines->push_back(expression(new Literal(next())));
                next();
        }
        return new Body(NULL,lines);
}



// check the token that is dist
// number ahead of the current token
Token* ASTGen::peek(int dist){
        if(m_pos+dist < m_tokens.size()){
                return m_tokens.at(m_pos+dist);
        }
        return NULL;
}

Token* ASTGen::peek(){
        return peek(0);
}


// return true if the type of token
// equals type 
bool ASTGen::equals(Token* token, TokenType type){
    if(token == NULL)
        return false; 
    return token->m_type == type;
}


// return true if 
// the token type is a 
// operators
bool ASTGen::isOP(Token* token){
        if(token ==nullptr)
                return false;
        switch(token->m_type){
                case TokenType::TIMES_TIMES:
                case TokenType::DIVIDE_DIVIDE:
                case TokenType::EQUAL_EQUAL:
                case TokenType::NOT_EQUAL:
                case TokenType::OR_OR:
                case TokenType::AND_AND:
                case TokenType::REMAND:
                case TokenType::DIVIDE:
                case TokenType::TIMES:
                case TokenType::MINUS:
                case TokenType::PLUS:return true;

        }
        return false;
}

//returns weather a token is
// */ != and so on
bool ASTGen::isMulti(Token* token){
        if(token == nullptr)
                return false;
        switch(token->m_type){
                case TokenType::TIMES:
                case TokenType::REMAND:
                case TokenType::DIVIDE:
                case TokenType::TIMES_TIMES:
                case TokenType::DIVIDE_DIVIDE: 
                 case TokenType::EQUAL_EQUAL:
                case TokenType::NOT_EQUAL: return true;
        }
        return false;
}


// return next token and if out of bounds return 
// null and increase m_pos
Token* ASTGen::next(){
   if(m_pos < m_tokens.size()){
        return m_tokens.at(m_pos++); 
    } 
    return NULL;
 
}


// Decide how a AST 
// should be constructed
Expression* ASTGen::expression(Expression* expr){
        if(equals(peek(),TokenType::SEMI_COLON) || equals(peek(),TokenType::CLOSE_PARENTHESE))
                return expr;
        if(isOP(peek())) 
                return binaryOperation(expr);
        if(expr->name() == "BinOP"){
                if(!isOP(peek()))
                        return expr;
                return binaryOperation(expr);
        }
        switch(((Literal*)expr)->m_token->m_type){
                case TokenType::VAR: return decleration((Literal*)expr, true);
                case TokenType::OPEN_PARENTHESE: return expression(new Literal(next()));
                case TokenType::IDEN: if(equals(peek(),TokenType::OPEN_PARENTHESE))
                                              return functionCall((Literal*)expr);
                case TokenType::INT: return expr; 

                case TokenType::IF: return body((Literal*)expr);
                case TokenType::NOT: 
                case TokenType::PLUS_PLUS:
                case TokenType::MINUS_MINUS: return expression(new Unary((Literal*)expr,new Literal(next())));
            }
        return NULL; 
}


// Construct a binOP based on specifically equations
// such as +-,*
Expression* ASTGen::binaryOperation(Expression* left){

       Token* op = next();
        Literal* right = new Literal(next());

       if(equals(peek(), TokenType::OPEN_PARENTHESE))
                       return expression(new BinOP(left,op,expression(right)));


       if(equals(peek(),TokenType::CLOSE_PARENTHESE)){
                        next();
                       return expression(new BinOP(left,op,right)); 

        }
 
        else if(isMulti(op)){
               return expression(new BinOP(left,op,right)); 



        }

        else{
               if(!isMulti(peek()) )
                       return expression(new BinOP(left,op,right)); 

                return new BinOP(left,op,expression(right));
        }
}

// Construct dec constructs declarations such as
// var name = 12
Decleration* ASTGen::decleration(Literal* type, bool initalize){
    Literal* name = new Literal(next());
    Token* ex = next();
    if(equals(ex,TokenType::SEMI_COLON)){
        next();
        return new Decleration(type, name, NULL,initalize);
    }
    return new Decleration(type, name, expression(new Literal(next())),initalize);
}


// construct a fucntion call 
// object based on parmaters
FunctionCall* ASTGen::functionCall(Literal* name){
        next();
        vector<Expression*>* args = new vector<Expression*>;
        if(!equals(peek(),TokenType::CLOSE_PARENTHESE)){
               do{
                        args->push_back(expression(new Literal(next())));          
               }while(equals(next(),TokenType::COMMA));
        }else{
                next();
        }
        return new FunctionCall(name,args);
}

//consturct a body based on
//paramters such as if while for and def
Body* ASTGen::body(Literal* type){
        IfStat* bod = new IfStat(expression(new Literal(next())));
        next();
        next();
        vector<Expression*>* lines = new vector<Expression*>();
        while(!equals(peek(),TokenType::CLOSE_BRACE)){
               lines->push_back(expression(new Literal(next())));
                next();
        }
       return new Body(bod,lines); 

}

// Created by Christopher Oosthuizen on 01/11/2021
// The AST generator is meant to return 
// A based off a list of tokens consturcted by a lexer
#include "ASTGen.h"
#include "AST.h"

ASTGen::ASTGen(vector<Token*> tokens){
    m_tokens = tokens;
    m_pos = 0;
}

/*
 * return a AST based on the tokens given in
 * the construction
 */
Expression* ASTGen::generateAST(){
     
    return constructOP(new Literal(next()));
}

/*
 * return the next token in the list 
 * without incrementing
 */
Token* ASTGen::peek(){
    if(m_pos < m_tokens.size()){
        return m_tokens.at(m_pos); 
    } 
    return NULL;
}

/*
 * return next token and if out of bounds return 
 * null and increase m_pos
 */
Token* ASTGen::next(){
   if(m_pos < m_tokens.size()){
        return m_tokens.at(m_pos++); 
    } 
    return NULL;
 
}

/*
 * return true if the type of token
 * equals type 
 */
bool ASTGen::equals(Token* token, TokenType type){
    return token->m_type == type;
}

/*
 * Construct a binOP based on specifically equations
 * such as +-,*
 */
Expression* ASTGen::constructOP(Expression* left){
    if(peek() == NULL)
        return left;
    Token* op = next();
    if(equals(op,TokenType::PLUS)|| equals(op, TokenType::MINUS)){
       return new BinOP(left,op,constructOP(new Literal(next()))); 
    }else if(equals(op,TokenType::TIMES) || equals(op,TokenType::DIVIDE)){
       return constructOP(new BinOP(left,op,new Literal(next())));
    }
}

// Created by Christopher Oosthuizen on 01/11/2021
// The AST generator is meant to return 
// A based off a list of tokens constructed by a lexer
#include "ASTGen.h"
#include "AST.h"
#include "ErrorThrower.h"


ASTGen::ASTGen(vector<Token*> tokens){
    m_tokens = tokens;
    m_pos = 0;
}

Body* ASTGen::generateAST(){
        vector<Expression*>* exprs = new vector<Expression*>();
        exprs->push_back(expression());
        return new Body(exprs);
}

Token* ASTGen::next(){
        if(m_pos >= m_tokens.size()){
                return nullptr;
        }
        return m_tokens[m_pos++];
}

Token* ASTGen::peek(){
        Token* token = next();
        m_pos--;
        return token;
}

Token* ASTGen::previous(){
        return m_tokens[m_pos-1];
}

int ASTGen::order(Token* token){
        switch(token->m_type){
                case TokenType::PLUS:
                case TokenType::MINUS:return 3;
                case TokenType::TIMES: 
                case TokenType::DIVIDE:return 2;
                case TokenType::TIMES_TIMES: 
                case TokenType::DIVIDE_DIVIDE:return 1;
                case TokenType::MINUS_MINUS:
                case TokenType::PLUS_PLUS:return 0;
        }
        return -1;
}

Expression* ASTGen::expression(){
        return binaryOperation(3);
}

Expression* ASTGen::binaryOperation(int precidence){
        
        if(precidence <= 0){
                return unary();
        } 
        Expression* expr = binaryOperation(precidence-1);

        while(order(peek()) == precidence){
                Token* op= next();
                Expression* right = binaryOperation(precidence-1);
                expr = new BinOP(expr,op,right);
        }
        return expr;
}

Expression* ASTGen::unary(){
        if(order(peek()) == 0 || peek()->m_type == TokenType::MINUS){
                Token* op = previous();
                Expression* right = unary();               
                return new Unary(new Literal(op), right,0);
        }
        return literal();
}

Expression* ASTGen::literal(){
        if(peek()->m_type == TokenType::OPEN_PARENTHESE){
                delete next();
                Expression* expr = expression();
                delete next();
                return expr; 
        }
        return new Literal(next());
}

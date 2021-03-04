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
        while(m_pos < m_tokens.size()){
                exprs->push_back(lineStat());
                delete next();
        }
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
               case TokenType::XOR:
                case TokenType::AND_AND:
                case TokenType::OR_OR: return 5;
                case TokenType::LESS_EQUAL:
                case TokenType::MORE_EQUAL:
                case TokenType::GREATER:
                case TokenType::LESS:
                case TokenType::EQUAL_EQUAL:
                case TokenType::NOT_EQUAL:return 4;
                case TokenType::PLUS:
                case TokenType::MINUS:return 3;
                case TokenType::TIMES: 
                case TokenType::DIVIDE:return 2;
                case TokenType::TIMES_TIMES: 
                case TokenType::DIVIDE_DIVIDE:return 1;
                case TokenType::MINUS_MINUS:
                case TokenType::NOT:
                case TokenType::PLUS_PLUS:return 0;
        }
        return -1;
}

bool ASTGen::isIden(Token* token){
        switch(token->m_type){
                case TokenType::VAR:
                case TokenType::IDEN_DOUBLE:
                case TokenType::IDEN_CHAR:
                case TokenType::IDEN_STRING:
                case TokenType::IDEN_INT:
                case TokenType::IDEN_BOOL:return 1;
        }
        return 0;
}

Expression* ASTGen::lineStat(){
        if(isIden(peek()) || (peek()->m_type == TokenType::IDEN&& m_tokens[m_pos+1]->m_type == TokenType::IDEN) ){
                return decleration();
        }
        if(m_pos+1 <m_tokens.size())
        return expression();
}

bool ASTGen::isEquals(Token* token){
        switch(token->m_type){
                case TokenType::EQUAL:
                case TokenType::TIMES_EQUAL:
                case TokenType::DIVIDE_EQUAL:
                case TokenType::PLUS_EQUAL:
                case TokenType::MINUS_EQUAL: return 1;
        }
        return 0;
}

Expression* ASTGen::expression(){

        Expression* expression = binaryOperation(6); 
        if(isEquals(peek())){
                expression = assignment(expression);
        }

        return expression; 
}

Expression* ASTGen::decleration(){
       Literal* type = new Literal(next()); 
       Expression* name = literal();
       Expression* value = nullptr;
       if(peek()->m_type == TokenType::EQUAL){
               delete next();
                value = expression();
       }
       return new Decleration(type,name,nullptr,value,true,false);
}

Expression* ASTGen::assignment(Expression* name){
       Literal* op = new Literal(next());
       Expression* value = expression();
       return new Decleration(nullptr,name,op,value,false,false);
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

        if(order(peek()) == 0 || peek()->m_type == TokenType::MINUS ){
                Token* op = next();
                Expression* right = unary();               
                return new Unary(new Literal(op), right,0);
        }
        return literal();
}

Expression* ASTGen::parans(){
        delete next();
        Expression* expr = expression();
        delete next();
        return expr; 

}

Expression* ASTGen::functionCall(){
        Literal* lit = new Literal(next());
        delete next();
        vector<Expression*>* args = new vector<Expression*>();
        while(peek()->m_type != TokenType::CLOSE_PARENTHESE){
                args->push_back(expression());
                if(peek()->m_type == TokenType::COMMA)
                        delete next();
        }
        delete next();
        return new FunctionCall(lit,args);
}

bool ASTGen::isFunc(Token* token){
        switch(token->m_type){
                case TokenType::IDEN:
                case TokenType::PRINT: return 1;
        }
        return 0;
}

Expression* ASTGen::literal(){
        if(peek()->m_type == TokenType::OPEN_PARENTHESE){
                return parans();
        }
        if(isFunc(peek()) &&m_tokens[m_pos+1]->m_type == TokenType::OPEN_PARENTHESE){
                return functionCall();
        }
        if(peek()->m_type == TokenType::NEW){
                delete next();
                return new New(new Literal(next()));
        }
        if(m_tokens[m_pos+1]->m_type == TokenType::DOT){
                Literal* name = new Literal(next());
                delete next();
                Literal* sub= new Literal(next());
                return new Dot(name,sub);
        }
        return new Literal(next());
}

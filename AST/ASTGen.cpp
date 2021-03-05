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
        while(m_pos < m_tokens.size() && peek()!= nullptr && !peek()->m_symbol.empty()){
                exprs->push_back(lineStat());
        }
        return new Body(nullptr, exprs);
}
void ASTGen::consume( TokenType type,const string& message ){
        Token* peeked = peek();
        if(!(peeked->m_type == type)){
                ErrorThrower::unNamedError(message,peeked->m_line);

        }else{
                delete next();

        }
}

Expression* ASTGen::body(){
       TokenType type = peek()->m_type;
       Literal* lit = new Literal(next());
       Expression* control= nullptr; 
       if(type == TokenType::ELSE){
               control = new ElseStat();
       }else if(type == TokenType::IF){
               control = new IfStat(expression());
       }else if(type == TokenType::FOR){
               delete next();
               Expression* inital = lineStat();
               Expression* controlStat = expression();
               delete next();
               Expression* rep = expression();
               delete next();
               control = new ForStat(inital,controlStat,rep);
       }else if(type == TokenType::WHILE){
               control = new WhileStat(expression()); 
       }else if(type == TokenType::STRUCT){
               control = new Struct(new Literal(next()));
       }else if(isIden(lit->m_token)){
                control = new Function(lit,(FunctionCall*)functionCall());
       }
       return new Body(control,lines());
}

vector<Expression*>* ASTGen::lines(){
        vector<Expression*>* exprs = new vector<Expression*>();
        if(peek()->m_type == TokenType::OPEN_BRACE){
                delete next();
                while(peek()->m_type != TokenType::CLOSE_BRACE){
                       exprs->push_back(lineStat());
                }
                delete next();
        }else{
                exprs->push_back(lineStat());
        }
        return exprs;
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
        if(token == NULL)
                return -1;
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
                case TokenType::VOID:
                case TokenType::IDEN_INT:
                case TokenType::IDEN_BOOL:return 1;
        }
        return 0;
}

bool ASTGen::isBod(Token* token){
        switch(token->m_type){
                case TokenType::FOR:
                case TokenType::IF:
                case TokenType::WHILE:
                case TokenType::ELSE:
                case TokenType::STRUCT:return 1;
        }
        return 0;

}

Expression* ASTGen::lineExpr(){
        Expression* expr = nullptr;
        bool isRange = m_pos+1 <m_tokens.size();
        bool isDec =isIden(peek()) || (isRange &&(m_tokens[m_pos+1]->m_type == TokenType::COLON || (peek()->m_type == TokenType::IDEN&& m_tokens[m_pos+1]->m_type == TokenType::IDEN))); 
        if(peek()->m_type ==TokenType::RETURN){
                delete next();
                expr = new Return(expression());
        }else if(peek()->m_type == TokenType::IMPORT){
                delete next();
                expr = new Import(expression());
        }else if( isBod(peek()) ||(isDec && m_tokens[m_pos+2]->m_type == TokenType::OPEN_PARENTHESE)){
                return body();
        }else if(isDec){
               expr = decleration(); 
        }else 
                 expr = expression();
        return expr;
 
}

Expression* ASTGen::lineStat(){
        Expression* expr = lineExpr();
        if(expr->name() !="Body")
                consume(TokenType::SEMI_COLON, "Error: Missing Semi Colon on line:");
        return expr;
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
        }else if(peek()->m_type == TokenType::PLUS_PLUS || peek()->m_type == TokenType::MINUS_MINUS){
                expression = new Unary(new Literal(next()), expression,true);
        }

        return expression; 
}

Expression* ASTGen::decleration(){
       Literal* type = new Literal(next());
       bool isArr = false; 
       if(peek()->m_type == TokenType::COLON){
               delete next();
               isArr = true;
       }
       Expression* name = literal();
       Expression* value = nullptr;
       if(peek()->m_type == TokenType::EQUAL){
               delete next();
                value = expression();
       }
       return new Decleration(type,name,nullptr,value,true,isArr);
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
                args->push_back(lineExpr());
                if(peek()->m_type == TokenType::COMMA)
                        delete next();
        }
        consume(TokenType::CLOSE_PARENTHESE,"Error: Unclosed function call on line:");
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
        bool isRange = m_pos+1 <m_tokens.size();
        if(isRange &&m_tokens[m_pos+1]->m_type == TokenType::DOT){
                Literal* name = new Literal(next());
                delete next();
                Literal* sub= new Literal(next());
                return new Dot(name,sub);
        }else if (isRange && m_tokens[m_pos+1]->m_type == TokenType::OPEN_BRACKET) {
                Literal* name = new Literal(next());
                delete next();
                Expression* value = nullptr;
                if(peek()->m_type != TokenType::CLOSE_BRACKET){
                        value = expression();
                }
                delete next();
                return new ArrayLiteral(name,value);
        }
        return new Literal(next());
}

// Created by Christopher Oosthuizen on 01/11/2021
// The AST generator is meant to return 
// A based off a list of tokens constructed by a lexer
#include "ASTGen.h"
#include "AST.h"


ASTGen::ASTGen(vector<Token*> tokens){
    m_tokens = tokens;
    m_pos = 0;
}

/*
 * construct a fucntion call 
 * object based on parmaters
 */
FunctionCall* ASTGen::constructFunctionCall(Literal* name){
        next();
        vector<Expression*>* args = new vector<Expression*>;
        if(!equals(peek(),TokenType::CLOSE_PARENTHESE)){
               do{
                        args->push_back(constructOP(new Literal(next())));          
               }while(equals(next(),TokenType::COMMA));
        }else{
                next();
        }
        return new FunctionCall(name,args);
}


/*
 * return a AST based on the tokens given in
 * the construction
 */
Body* ASTGen::generateAST(){
        vector<Expression*>* lines = new vector<Expression*>;
        while(peek() != NULL){
                lines->push_back(constructEx());
        }
        return new Body(NULL,lines);
}

/*
 * return the next token in the list 
 * without incrementing
 */
Token* ASTGen::peek(){
        return peek(0);
}

/*
 * check the token that is dist
 * number ahead of the current 
 * token
 */
Token* ASTGen::peek(int dist){
        if(m_pos+dist < m_tokens.size()){
                return m_tokens.at(m_pos+dist);
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
    if(token == NULL)
        return false; 
    return token->m_type == type;
}

/*
 * Decide how a AST 
 * should be constructed
 */
Expression* ASTGen::constructEx(){
    Token* curr = next();
    switch(curr->m_type){
        case TokenType::VAR: return constructDec(true);
        case TokenType::OPEN_PARENTHESE: return constructOP(new Literal(next()));
        case TokenType::INT: 
        case TokenType::IDEN:return constructOP(new Literal(curr)); 

    }
    return NULL; 
}

/*
 * Construct dec constructs declarations such as
 * var name = 12
 * or name =12
 */
Decleration* ASTGen::constructDec(bool initalize){
    Literal* name = new Literal(next());
    Token* ex = next();
    if(equals(ex,TokenType::SEMI_COLON)){
        next();
        return new Decleration(name, NULL,initalize);
    }
    return new Decleration(name, constructEx(),initalize);
}

/*
 * Construct a binOP based on specifically equations
 * such as +-,*
 */
Expression* ASTGen::constructOP(Expression* left){

        if(left->name() == "Literal" && equals(((Literal*)left)->m_token,TokenType::IDEN)&&equals(peek(), TokenType::OPEN_PARENTHESE))
                        return constructFunctionCall((Literal*)left);


        if(equals(peek(),TokenType::COMMA) || equals(peek(),TokenType::CLOSE_PARENTHESE))
                return left;

        Token* op = next();
        string type = left->name();
        if(equals(op,TokenType::SEMI_COLON)  ){
                return left;
        }



        if(equals(peek(1),TokenType::CLOSE_PARENTHESE) ){
                Literal* right = new Literal(next());
                next();
                return constructOP(new BinOP(left,op,right)); 
        }


        switch(op->m_type){
                case TokenType::PLUS:
                case TokenType::MINUS: return new BinOP(left,op,constructOP(new Literal(next()))); 
                case TokenType::OPEN_PARENTHESE: next();
                case TokenType::TIMES:
                case TokenType::REMAND:
                case TokenType::DIVIDE: return constructOP(new BinOP(left,op,new Literal(next())));

        }   
        return NULL;
}



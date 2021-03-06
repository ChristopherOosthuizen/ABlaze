/*
 * Created by Christopher Oosthuizen on 11/01/2021
 * This files purpose is to generate a ast based on
 *  credentials.
 */
 
#ifndef ABLAZE_ASTGEN_H
#define ABLAZE_ASTGEN_H
#include "Token.h"
#include "AST.h"
#include <vector>

class ASTGen{
    vector<Token*> m_tokens;
    int m_pos;
    public:
        ASTGen(vector<Token*> tokens);
        Body* generateAST(); //return a ast based on nthe inputed tokens
    private:
        Token* peek();// check the token infront of the current
        Token* next(); 
        Token* previous();
        int order(Token* token);
        bool isIden(Token* token);
        bool isEquals(Token* token);
        bool isBod(Token* token);
        vector<Expression*>* lines();
        Expression* expression(); // construct a AST based on Predefined paramaters
        void consume(TokenType type, const string& message);
        Expression* literal();
        Expression* parans();
        Expression* binaryOperation(int precidence); // Constructed a binOP AST 
        Expression* unary();
        Expression* body();
        Expression* decleration();
        Expression* assignment(Expression* name);
        Expression* functionCall();
        Expression* lineStat();
        Expression* lineExpr();
        Expression* postfix();
        Expression* builtIn();
        bool isFunc(Token* token);

};

#endif

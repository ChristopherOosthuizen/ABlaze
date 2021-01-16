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
        Token* peek(int dist); // check the token dist number ahead
        Token* next(); // return the next token
        bool equals(Token* token, TokenType type); // returns weather a a token's type matches a type
        Expression* constructOP(Expression* left); // Constructed a binOP AST 
        Expression* constructEx(); // construct a AST based on Predefined paramaters
        Decleration* constructDec(bool initalize); // constucts a decleration AST type

};

#endif

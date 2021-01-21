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
        Token* peek(int dist); // check the token dist number ahead
        Token* peek();// check the token infront of the current
        Token* next(); 
        bool equals(Token* token, TokenType type); // returns weather a a token's type matches a type
        bool isOP(Token* token);  // returns weather a token is a op such as + - * /
        bool isMulti(Token* token); // returns weather it is higher prority
        Expression* expression(Expression* expr); // construct a AST based on Predefined paramaters
        Expression* binaryOperation(Expression* left); // Constructed a binOP AST 
        Decleration* decleration(Literal* type, bool initalize); // constucts a decleration AST type
        FunctionCall* functionCall(Literal* name); // construct a function call object
        Body* body(Literal* type); // cosntruct a body such as if while for and def

};

#endif

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
        Token* next(); // returns the next token without stepping forward
        bool equals(TokenType type); //returns weather the next is a token
        bool equalsForward(int i,TokenType type); // equals one infront of current
        bool eat(TokenType type); //if peek() equals the token eat it and return true
        void consume(TokenType type, const string& message); //consumes the next if not equal to specified throw the error given
        int order(Token* token); //returns the order of precidence of a token and -1 other wise
        bool isIden(Token* token); //returns true if the tokens is a identifier
        bool isEquals();//returns true if the token contaisn =
        bool isBod(); //returns weather a tokenType is a structure identifier
        bool isFunc(); //return weather a tokenType is a builtin function
        vector<Expression*>* lines(); // returns lines based on the {}
        Expression* expression(); // construct a AST based on Predefined paramaters
        Expression* literal(); // returns next literal value
        Expression* parans(); // returns expression from partenthese
        Expression* binaryOperation(int precidence); // Constructed a binOP AST 
        Expression* unary(); // turns unary operators into Expressions
        Expression* body(); // handles both the body and controll statment
        Expression* decleration(); // converts a decleration to a token
        Expression* assignment(Expression* name); // convert a assignment into a decleration
        Expression* functionCall(); //convert functioncall
        Expression* lineStat(); // turns expression assuming that it will end with ;
        Expression* lineExpr(); // lineStat but does not assume semicolon
        Expression* postfix(); // turns postfix into a expression
        Expression* builtIn(); // deals with builtin functions such as return
        Array* array();
        Literal* nextLit(); //return new new Literal(next());

};

#endif

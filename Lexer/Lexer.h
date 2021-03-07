//
// Created by Chris Oosthuizen on 12/20/20.
// The lexer class takes in a string and converts it into tokens
// to be used later
//

#ifndef ABLAZE_LEXER_H
#define ABLAZE_LEXER_H
#include "Token.h"
#include "TokenType.h"
#include <vector>
#include <map>

class Lexer {
    map<string,TokenType> m_built_in_words;// idnetifes any bultin keywords and there type
    int m_pos;
    int m_line;
    string m_input;


public:
    Lexer(const string& input);
    vector<Token*> readAllTokens(); //reads turns all of input into Tokens then returns.
    Token* next();		   //returns the token at pos and increments

private:
    char peek();     // returns the character one in front of pos
    bool isAtEnd();  //tells weather pos is at the end
    bool isNum(char c); //returns weather char is a int
    bool isLetter(char c); //returns weather a char is a letter
    bool peekIs(char c); // return true if the peek equals a char
    bool consume(char c); //if peekIs m_pos pluss plus as well
    Token* strings(char ender,TokenType resultType); // extract all of next token;
    Token* Double(); // convert a double to a token
    Token* Integer(); // convert a integer to a token
    Token* Iden(); // convert identifiers and reserved keywords to correct token type
};

#endif //ABLAZE_LEXER_H

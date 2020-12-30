//
// Created by Chris Oosthuizen on 12/20/20.
// The lexer class takes in a string and converts it into tokens
// to be used later
//

#ifndef ABLAZE_LEXER_H
#define ABLAZE_LEXER_H
#include "Token.h"
#include <vector>

class Lexer {
    int m_pos;
    int m_line;
    string m_input;

public:
    Lexer(const string& input);
    vector<Token> readAllTokens(); //reads turns all of input into Tokens then returns.
    Token next();		   //returns the token at pos and increments

private:
    char peek();     // returns the character one in front of pos
    bool isAtEnd();  //tells weather pos is at the end
    Token strings(); // extract all of next token;
};

#endif //ABLAZE_LEXER_H

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
    Lexer(string input);
    vector<Token> readAllTokens;

private:
    Token next();
    char peek();

};


#endif //ABLAZE_LEXER_H

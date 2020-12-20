//
// Created by chris on 12/20/20.
// The Token class is designed to hold data about
// specific characters of interest such as
// a '+' representing a PLUS type
// and to hold info on it.
//

#ifndef ABLAZE_TOKEN_H
#define ABLAZE_TOKEN_H
#include <string>
#include "TokenType.h"
using namespace std;



class Token {
public:
    TokenType m_type;
    int m_line;
    string m_symbol;

    Token(TokenType type, string symbol, int line){
        m_type = type;
        m_symbol = symbol;
        m_line = line;

    }

    string toString(){
        return type +" "+ symbol+ " "+ line;
    }
};


#endif //ABLAZE_TOKEN_H

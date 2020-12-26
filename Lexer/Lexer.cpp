//
// Created by chris on 12/20/20.
//

#include "Lexer.h"

Lexer::Lexer(const string& input) {
    m_line =0;
    m_pos =0;
    m_input = input+"\0";
}

/*
 * reads all the tokens from the inputted string
 * in the constructor.
 */
vector<Token> Lexer::readAllTokens(){
    vector<Token> tokens;
    while(!isAtEnd()){
       tokens.push_back(next());
    }
    return tokens;
}

/*
 * converts the char at m_pos
 * into a Token and increments pos
 *
 */
Token Lexer::next() {
    if(isAtEnd()){
        return Token(TokenType::END, "", m_line);
    }
    char current = m_input.at(m_pos);
    m_pos++;
    switch (current) {
        case '+':return Token(TokenType::PLUS,"+",m_line);
        case '-':return Token(TokenType::MINUS,"-",m_line);
        case '*':return Token(TokenType::TIMES,"*",m_line);
        case '/':return Token(TokenType::DIVIDE,"/",m_line);
        case '&':return Token(TokenType::AND,"&",m_line);
        case '|':return Token(TokenType::OR,"|",m_line);
        case ':':return Token(TokenType::COLON,":",m_line);
        case ';':return Token(TokenType::SEMI_COLON,";",m_line);
        case '<':return Token(TokenType::LESS,"<",m_line);
        case '>':return Token(TokenType::GREATER,">",m_line);
        case '{':return Token(TokenType::OPEN_BRACE,"{",m_line);
        case '}':return Token(TokenType::CLOSE_BRACE,"}",m_line);
        case '[':return Token(TokenType::OPEN_BRACKET,"[",m_line);
        case ']':return Token(TokenType::CLOSE_BRACKET,"]",m_line);
        case '(':return Token(TokenType::OPEN_PARENTHESE,"(",m_line);
        case ')':return Token(TokenType::CLOSE_PARENTHESE,")",m_line);
        case ',':return Token(TokenType::COMMA,",",m_line);
        case '.':return Token(TokenType::DOT,".",m_line);
        case '!':return Token(TokenType::NOT,"!",m_line);

        case ' ':return next();
        default: return Token(TokenType::END, "", m_line);
    }

}

/*
 *return the next character
 * while not incrementing
 */
char Lexer::peek() {
    if(m_input.size() > m_pos){
        return m_input.at(m_pos);
    }
    return '\0';
}

/*
 * Safely tell weather the m_pos is
 * at the end or past the end of the string
 */
bool Lexer::isAtEnd() {
   return m_input.length() <= m_pos || m_input.at(m_pos) == '\0';
}

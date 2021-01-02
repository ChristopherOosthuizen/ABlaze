//
// Created by chris on 12/20/20.
//

#include "Lexer.h"
#include "ErrorThrower.h"
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
        case '+':
		if(peek() == '+'){
			m_pos++;
			return Token(TokenType::PLUS_PLUS,"++",m_line);
		}else if(peek() == '='){
			m_pos++;
			return Token(TokenType::PLUS_EQUAL,"+=",m_line);
		}

		return Token(TokenType::PLUS,"+",m_line);

        case '-':
		if(peek() == '-'){
			m_pos++;
			return Token(TokenType::MINUS_MINUS,"--",m_line);
		}else if(peek() == '='){
			m_pos++;
			return Token(TokenType::MINUS_EQUAL,"-=",m_line);
		}

		return Token(TokenType::MINUS,"-",m_line);

        case '*':
		if(peek() == '*'){
			m_pos++;
			return Token(TokenType::TIMES_TIMES,"**",m_line);
		}else if(peek() == '='){
			m_pos++;
			return Token(TokenType::TIMES_EQUAL,"*=",m_line);
		}

		return Token(TokenType::TIMES,"*",m_line);

        case '/':
		if(peek() == '/'){
			m_pos++;
			return Token(TokenType::DIVIDE_DIVIDE,"//",m_line);
		}else if(peek() == '='){
			m_pos++;
			return Token(TokenType::DIVIDE_EQUAL,"/=",m_line);
		}
		return Token(TokenType::DIVIDE,"/",m_line);

        case '&':
		if(peek() == '&'){
			m_pos++;
			return Token(TokenType::AND_AND,"&&",m_line);
		}
		return Token(TokenType::AND,"&",m_line);

        case '|':
		if(peek() == '|'){
			m_pos++;
			return Token(TokenType::OR_OR,"||",m_line);
		}
		return Token(TokenType::OR,"|",m_line);
	
	case '=':
		 if(peek() == '='){
			 m_pos++;
			 return Token(TokenType::EQUAL_EQUAL,"==",m_line);
		 }
		return Token(TokenType::EQUAL,"=",m_line);
	
        case '<':
		if(peek() == '='){
			m_pos++;
			return Token(TokenType::LESS_EQUAL,"<=",m_line);
		}
		return Token(TokenType::LESS,"<",m_line);

        case '>':
		if(peek() == '='){
			m_pos++;
			return Token(TokenType::MORE_EQUAL,"<=",m_line);
		}
		return Token(TokenType::GREATER,">",m_line);

        case '!':
		if(peek() == '='){
			m_pos++;
			return Token(TokenType::NOT_EQUAL,"!=",m_line);
		}
		return Token(TokenType::NOT,"!",m_line);
 
        case ':':return Token(TokenType::COLON,":",m_line);
        case ';':return Token(TokenType::SEMI_COLON,";",m_line);
	case '{':return Token(TokenType::OPEN_BRACE,"{",m_line);
        case '}':return Token(TokenType::CLOSE_BRACE,"}",m_line);
        case '[':return Token(TokenType::OPEN_BRACKET,"[",m_line);
        case ']':return Token(TokenType::CLOSE_BRACKET,"]",m_line);
        case '(':return Token(TokenType::OPEN_PARENTHESE,"(",m_line);
        case ')':return Token(TokenType::CLOSE_PARENTHESE,")",m_line);
        case ',':return Token(TokenType::COMMA,",",m_line);
        case '.':return Token(TokenType::DOT,".",m_line);
        case ' ':return next();
	case '"': return strings();
	default: ErrorThrower::invalidToken(current,m_line);
		  return Token(TokenType::END, "", m_line);
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

/*
 * Convert next string into a token 
 * ending once a non excaped sequenced quote 
 * it given
 */
Token Lexer::strings(){
	int start = m_pos;	
	while(m_pos < m_input.length() && peek() != '"'){
		if(peek() == '\n')
			m_line++;
		m_pos++;
	}
	return Token(TokenType::STRING,m_input.substr(start, m_pos-start),m_line);
}

//
// Created by chris on 12/20/20.
//

#include "Lexer.h"
#include "ErrorThrower.h"
#include "TokenType.h"
#include <iostream>


Lexer::Lexer(const string& input) {
    m_line =1;
    m_pos =0;
    m_input = input+"\0";
		if(m_built_in_words.size() ==0){
			m_built_in_words["print"] = TokenType::PRINT;
			m_built_in_words["println"] = TokenType::PRINTLN;
			m_built_in_words["import"] = TokenType::IMPORT;
			m_built_in_words["while"] = TokenType::WHILE;
			m_built_in_words["for"] = TokenType::FOR;
			m_built_in_words["if"] = TokenType::IF;
			m_built_in_words["var"] = TokenType::VAR;
			m_built_in_words["return"] = TokenType::RETURN;
			m_built_in_words["void"] = TokenType::VOID;
			m_built_in_words["int"] = TokenType::IDEN_INT;
			m_built_in_words["double"] = TokenType::IDEN_DOUBLE;
			m_built_in_words["bool"] = TokenType::IDEN_BOOL;
			m_built_in_words["string"] = TokenType::IDEN_STRING;
			m_built_in_words["true"] = TokenType::BOOL;
			m_built_in_words["false"] = TokenType::BOOL;
			m_built_in_words["else"] = TokenType::ELSE;
			m_built_in_words["char"] = TokenType::IDEN_CHAR;
			m_built_in_words["struct"] = TokenType::STRUCT;
			m_built_in_words["new"] = TokenType::NEW;
			m_built_in_words["class"] = TokenType::CLASS;
			m_built_in_words["extends"] = TokenType::EXTENDS;
			m_built_in_words["nil"] = TokenType::NIL;
			m_built_in_words["static"] = TokenType::STATIC;
			m_built_in_words["len"] = TokenType::LEN;

		}
}

// return weather a char is between 0-9
bool Lexer::isNum(char c){
	bool is =c >47 && c <=57;  
	return is; 
}

// check if weather a char is a letter
bool Lexer::isLetter(char c){
	bool is = c== '_'|| (c >=65 && c< 91) || (c >= 97 && c < 122);
	return is;
}

//return true if peek equals char
bool Lexer::peekIs(char c){
	return peek() ==c;
}

bool Lexer::consume(char c){
	if(peekIs(c)){
		m_pos++;
		return 1;
	}
	return 0;
}

/*
 * reads all the tokens from the inputted string
 * in the constructor.
 */
vector<Token*> Lexer::readAllTokens(){
    vector<Token*> tokens;
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
Token* Lexer::next() {
    if(isAtEnd()){
        return new Token(TokenType::END, "", m_line);
    }
    char current = m_input.at(m_pos++);
    switch (current) {
		case '%':
		if(consume('='))return new Token(TokenType::MOD_EQUAL,"%=",m_line);
		return new Token(TokenType::REMAND,"%",m_line);

        case '+':
		if(consume('+'))return new Token(TokenType::PLUS_PLUS,"++",m_line);
		else if(consume('='))return new Token(TokenType::PLUS_EQUAL,"+=",m_line);
		return new Token(TokenType::PLUS,"+",m_line);

        case '-':
		if(consume('-'))return new Token(TokenType::MINUS_MINUS,"--",m_line);
		else if(consume('='))return new Token(TokenType::MINUS_EQUAL,"-=",m_line);
		return new Token(TokenType::MINUS,"-",m_line);

        case '*':
		if(consume('*'))return new Token(TokenType::TIMES_TIMES,"**",m_line);
		else if(consume('='))return new Token(TokenType::TIMES_EQUAL,"*=",m_line);
		return new Token(TokenType::TIMES,"*",m_line);

        case '/':
		if(consume('/'))return new Token(TokenType::DIVIDE_DIVIDE,"//",m_line);
		else if(consume('='))return new Token(TokenType::DIVIDE_EQUAL,"/=",m_line);
		return new Token(TokenType::DIVIDE,"/",m_line);

        case '&':
		if(consume('&'))return new Token(TokenType::AND_AND,"&&",m_line);
		return new Token(TokenType::AND,"&",m_line);
		case '^':
		return new Token(TokenType::XOR,"^",m_line);

        case '|':
		if(consume('|'))return new Token(TokenType::OR_OR,"||",m_line);
		return new Token(TokenType::OR,"|",m_line);
	
		case '=':
		 if(consume('='))return new Token(TokenType::EQUAL_EQUAL,"==",m_line);
		return new Token(TokenType::EQUAL,"=",m_line);
	
        case '<':
		if(consume('<'))return new Token(TokenType::LEFT_SHIFT,"<<",m_line);
		if(consume('='))return new Token(TokenType::LESS_EQUAL,"<=",m_line);
		return new Token(TokenType::LESS,"<",m_line);

        case '>':
		if(consume('>'))return new Token(TokenType::RIGHT_SHIFT,">>",m_line);
		if(consume('='))return new Token(TokenType::MORE_EQUAL,">=",m_line);
		return new Token(TokenType::GREATER,">",m_line);

        case '!':
		if(consume('='))return new Token(TokenType::NOT_EQUAL,"!=",m_line);
		return new Token(TokenType::NOT,"!",m_line);
 
        case ':':return new Token(TokenType::COLON,":",m_line);
        case ';':return new Token(TokenType::SEMI_COLON,";",m_line);
		case '{':return new Token(TokenType::OPEN_BRACE,"{",m_line);
        case '}':return new Token(TokenType::CLOSE_BRACE,"}",m_line);
        case '[':return new Token(TokenType::OPEN_BRACKET,"[",m_line);
        case ']':return new Token(TokenType::CLOSE_BRACKET,"]",m_line);
        case '(':return new Token(TokenType::OPEN_PARENTHESE,"(",m_line);
        case ')':return new Token(TokenType::CLOSE_PARENTHESE,")",m_line);
        case ',':return new Token(TokenType::COMMA,",",m_line);
        case '.': if(isNum(peek())){
				m_pos--;
				return Double();
			}
			 return new Token(TokenType::DOT,".",m_line);

        case ' ':return next();
		case '\t': return next();
		case '\r': return next();
        case '\n': m_line++;
                   return next();
		case '\0': return new Token(TokenType::END,"\0",m_line);
		case '"': return strings('"',TokenType::STRING);
		case '\'': return strings('\'',TokenType::CHAR);
		case '#': while(m_pos < m_input.size() && m_input.at(m_pos) != '\n') m_pos++; return next();
	
		default: 
		  if(isNum(current)){
			  return Integer();
			}else if(isLetter(current)){
				return Iden();
			}
			string chas ="";
			chas += current;
		  ErrorThrower::error(m_line,"Unkown character: "+chas);
		  return new Token(TokenType::END, "", m_line);
    }
	

}

/*
 *return the next character
 * while not incrementing
 */
char Lexer::peek() {
    if(!isAtEnd()){
        return m_input.at(m_pos);
    }
    return '\0';
}

/*
 * Safely tell weather the m_pos is
 * at the end or past the end of the string
 */
bool Lexer::isAtEnd() {
   return m_input.length() <= m_pos ;
}

/*
 * Convert next string into a token 
 * ending once a non excaped sequenced quote 
 * it given
 */
Token* Lexer::strings(char ender,TokenType type){
	int start = m_pos;	
	bool isEsc = false;
	int line = m_line;
	for(;!peekIs('\0') &&!peekIs(ender)||isEsc; m_pos++){ 
		if(peekIs('\n'))
			m_line++;
		isEsc = m_input.at(m_pos) == '\\';
	}

	if(isAtEnd())ErrorThrower::error(line,"Unterminated string");
	
	m_pos++;
	string result =m_input.substr(start, (m_pos-1)-start); 
	return new Token(type,result,m_line);
}

/*
 * Starting at pos this function converts 
 * a double to a token
 */
Token* Lexer::Double(){
	int start = m_pos;
	bool hasDot = false;
	while( peekIs('.')||isNum(peek())){	
			m_pos++;
			if(peekIs('.') ){
				if(hasDot)
					ErrorThrower::error(m_line,"Double can not contain two dots");
				else 
					hasDot =true;
			}

	}
	return new Token(TokenType::DOUBLE,m_input.substr(start, m_pos-start),m_line);
	
}

/*
 * Starting at pos return this function returns
 * a int turned into a token
 */
Token* Lexer::Integer(){
	int start = m_pos-1;
	while( isNum(peek()))m_pos++;
	
	if(peekIs('.')){
		m_pos = start;
		return  Double();
	}

	return new Token(TokenType::INT,m_input.substr(start, m_pos-start),m_line);

}

//convert iden to token
Token* Lexer::Iden(){
	int start = m_pos-1;
	while( isLetter(peek()) ||isNum(peek()))m_pos++;
	
	string word = m_input.substr(start, m_pos-start);
	if(m_built_in_words.find(word) != m_built_in_words.end()  ){
		TokenType type = m_built_in_words[word];
		return new Token(type,word,m_line);
	}

	return new Token(TokenType::IDEN,word,m_line);
}

//
// Created by chris on 12/20/20.
//

#include "Lexer.h"
#include "ErrorThrower.h"
#include "TokenType.h"

Lexer::Lexer(const string& input) {
    m_line =0;
    m_pos =0;
    m_input = input+"\0";
		if(m_built_in_words.size() ==0){
			m_built_in_words["print"] = TokenType::PRINT;
			m_built_in_words["println"] = TokenType::PRINTLN;
			m_built_in_words["import"] = TokenType::IMPORT;
			m_built_in_words["readFile"] = TokenType::READFILE;
			m_built_in_words["writeFile"] = TokenType::WRITEFILE;
			m_built_in_words["input"] = TokenType::INPUT;
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

		}
}

// return weather a char is between 0-9
bool isNum(char c){
	bool is =c >47 && c <=57;  
	return is; 
}

// check if weather a char is a letter
bool isLetter(char c){
	bool is = c== '_'|| (c >=65 && c< 91) || (c >= 97 && c < 122);
	return is;
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

string Lexer::typeToString(TokenType type){
	switch(type){
		case TokenType::PLUS:return "PLUS";
		case TokenType::MINUS  :return "MINUS";
		case TokenType::TIMES  :return "TIMES";
		case TokenType::DIVIDE  :return "DIVIDE";
		case TokenType::REMAND :return "REMAN";
		case TokenType::PLUS_PLUS  :return "PLUS_PLUS";
		case TokenType::MINUS_MINUS  :return "MINUS_MINUS";
		case TokenType::TIMES_TIMES :return "TIMES_TIME";
		case TokenType::DIVIDE_DIVIDE :return "DIVIDE_DIVID";
		case TokenType::EQUAL :return "EQUAL";
		case TokenType::EQUAL_EQUAL :return "EQUAL_EQUAL";
		case TokenType::PLUS_EQUAL :return "PLUS_EQUAL";
		case TokenType::MINUS_EQUAL :return "MINUS_EQUAL";
		case TokenType::TIMES_EQUAL :return "TIMES_EQUAL";
		case TokenType::DIVIDE_EQUAL :return "DIVIDE_EQUAL";
		case TokenType::LESS_EQUAL :return "LESS_EQUAL";
		case TokenType::MORE_EQUAL :return "MORE_EQUAL";
		case TokenType::NOT_EQUAL :return "NOT_EQUAL";
		case TokenType::MOD_EQUAL :return "MOD_EQUAL";
		case TokenType::AND :return "AND";
		case TokenType::OR :return "OR";
		case TokenType::XOR :return "XOR";
		case TokenType::NOT :return "NOT";
		case TokenType::GREATER :return "GREATER";
		case TokenType::LESS :return "LESS";
		case TokenType::OR_OR :return "OR_OR";
		case TokenType::AND_AND :return "AND_AND";
		case TokenType::INT: return "INT";  
		case TokenType::DOUBLE : return "DOUBLE";  
		case TokenType::CHAR : return "CHAR";  
		case TokenType::STRING : return "STRING";  
		case TokenType::BOOL : return "BOOL";  
		case TokenType::IDEN : return "IDEN";  
		case TokenType::OPEN_PARENTHESE : return "OPEN_PARENTHESE";  
		case TokenType::CLOSE_PARENTHESE : return "CLOSE_PARENTHESE";  
		case TokenType::OPEN_BRACKET : return "OPEN_BRACKET";  
		case TokenType::CLOSE_BRACKET : return "CLOSE_BRACKET";  
		case TokenType::OPEN_BRACE : return "OPEN_BRACE";  
		case TokenType::CLOSE_BRACE : return "CLOSE_BRACE";  
		case TokenType::SEMI_COLON : return "SEMI_COLON";  
		case TokenType::COLON : return "COLON";  
		case TokenType::DOT : return "DOT";  
		case TokenType::COMMA : return "COMMA";  
		case TokenType::END : return "END";  
		case TokenType::PRINT : return "PRINT";  
		case TokenType::PRINTLN : return "PRINTLN";  
		case TokenType::IMPORT : return "IMPORT";  
		case TokenType::READFILE : return "READFILE";  
		case TokenType::WRITEFILE : return "WRITEFILE";  
		case TokenType::INPUT : return "INPUT";  
		case TokenType::VAR : return "VAR";  
		case TokenType::VOID : return "VOID";  
		case TokenType::RETURN : return "RETURN";  
		case TokenType::IF : return "IF";  
		case TokenType::FOR : return "FOR";  
		case TokenType::WHILE : return "WHILE";  
		case TokenType::IDEN_INT : return "IDEN_INT";  
		case TokenType::IDEN_BOOL : return "IDEN_BOOL";  
		case TokenType::IDEN_STRING : return "IDEN_STRING";  
		case TokenType::IDEN_DOUBLE : return "IDEN_DOUBLE";  
	}
	return "";
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
    char current = m_input.at(m_pos);
    m_pos++;
    switch (current) {
			case '%':
                        if(peek() == '='){
                                m_pos++;
                                return new Token(TokenType::PLUS_PLUS,"%=",m_line);
		}
				return new Token(TokenType::REMAND,"%",m_line);
        case '+':
		if(peek() == '+'){
			m_pos++;
			return new Token(TokenType::PLUS_PLUS,"++",m_line);
		}else if(peek() == '='){
			m_pos++;
			return new Token(TokenType::PLUS_EQUAL,"+=",m_line);
		}

		return new Token(TokenType::PLUS,"+",m_line);

        case '-':
		if(peek() == '-'){
			m_pos++;
			return new Token(TokenType::MINUS_MINUS,"--",m_line);
		}else if(peek() == '='){
			m_pos++;
			return new Token(TokenType::MINUS_EQUAL,"-=",m_line);
		}

		return new Token(TokenType::MINUS,"-",m_line);

        case '*':
		if(peek() == '*'){
			m_pos++;
			return new Token(TokenType::TIMES_TIMES,"**",m_line);
		}else if(peek() == '='){
			m_pos++;
			return new Token(TokenType::TIMES_EQUAL,"*=",m_line);
		}

		return new Token(TokenType::TIMES,"*",m_line);

        case '/':
		if(peek() == '/'){
			m_pos++;
			return new Token(TokenType::DIVIDE_DIVIDE,"//",m_line);
		}else if(peek() == '='){
			m_pos++;
			return new Token(TokenType::DIVIDE_EQUAL,"/=",m_line);
		}
		return new Token(TokenType::DIVIDE,"/",m_line);

        case '&':
		if(peek() == '&'){
			m_pos++;
			return new Token(TokenType::AND_AND,"&&",m_line);
		}
		return new Token(TokenType::AND,"&",m_line);

        case '|':
		if(peek() == '|'){
			m_pos++;
			return new Token(TokenType::OR_OR,"||",m_line);
		}
		return new Token(TokenType::OR,"|",m_line);
	
	case '=':
		 if(peek() == '='){
			 m_pos++;
			 return new Token(TokenType::EQUAL_EQUAL,"==",m_line);
		 }
		return new Token(TokenType::EQUAL,"=",m_line);
	
        case '<':
		if(peek() == '='){
			m_pos++;
			return new Token(TokenType::LESS_EQUAL,"<=",m_line);
		}
		return new Token(TokenType::LESS,"<",m_line);

        case '>':
		if(peek() == '='){
			m_pos++;
			return new Token(TokenType::MORE_EQUAL,"<=",m_line);
		}
		return new Token(TokenType::GREATER,">",m_line);

        case '!':
		if(peek() == '='){
			m_pos++;
			return new Token(TokenType::NOT_EQUAL,"!=",m_line);
		}
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
        case '\n': m_line++;
                   return next();
	case '\0': return new Token(TokenType::END,"\0",m_line);
	case '"': return strings();
	
	default: 
		  if(isNum(current)){
			  return number();
			}else if(isLetter(current)){
				return Iden();
			}
		  ErrorThrower::invalidToken(current,m_line);
		  return new Token(TokenType::END, "", m_line);
    }

}

/*
 *return the next character
 * while not incrementing
 */
char Lexer::peek() {
    if(m_input.length() > m_pos){
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
Token* Lexer::strings(){
	int start = m_pos;	
	bool isEsc = false;
	char peeked = peek();
	while(m_pos < m_input.length() && (peeked != '"' || isEsc)){ 
		if(peeked == '\n')
			m_line++;
		m_pos++;
		isEsc = m_input.at(m_pos-1) == '\\';
		peeked = peek();
	}
	if(m_pos >= m_input.length() ){
		ErrorThrower::unterminatedString(m_line);
	}
        m_pos++;
	return new Token(TokenType::STRING,m_input.substr(start, (m_pos-1)-start),m_line);
}

/*
 * distinguish between double and int
 * and return there value
 */
Token* Lexer::number(){
	return Integer();
}

/*
 * Starting at pos this function converts 
 * a double to a token
 */
Token* Lexer::Double(){
	int start = m_pos;
	char c = m_input.at(m_pos);
	bool hasDot = false;
	while( peek() == '.'||isNum(peek())){	
			m_pos++;
			if(peek() == '.' && hasDot)
				ErrorThrower::doubleDot(m_line);
			else if(peek() == '.')
				hasDot =true;

	}


	return new Token(TokenType::DOUBLE,m_input.substr(start, m_pos-start),m_line);
	
}

/*
 * Starting at pos return this function returns
 * a int turned into a token
 */
Token* Lexer::Integer(){
	int start = --m_pos;
	char c = m_input.at(m_pos);
	while( isNum(peek())){	
			m_pos++;
	}
	if(peek() == '.'){
		m_pos = start;
		return  Double();
	}


	return new Token(TokenType::INT,m_input.substr(start, m_pos-start),m_line);

}

Token* Lexer::Iden(){
	int start = --m_pos;
	char c = m_input.at(m_pos);
	while( isLetter(peek())){	
			m_pos++;
	}
	string word = m_input.substr(start, m_pos-start);
		if(m_built_in_words.find(word) != m_built_in_words.end()  ){
			return new Token(m_built_in_words[word],word,m_line);
			
		}

	return new Token(TokenType::IDEN,word,m_line);
}

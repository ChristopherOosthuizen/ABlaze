//printValue
// Created by chris on 12/20/20.
// The specific types of a Token
// ex: + is PLUS
// ex: & is a and
// ex: += is a PLUS_EQUAL
//

#ifndef ABLAZE_TOKENTYPE_H
#define ABLAZE_TOKENTYPE_H
#include <string>
#include <vector>
using namespace std;
enum class TokenType{
  // Operators
  PLUS , MINUS , TIMES , DIVIDE , REMAND,

  // DOUBLE Operators
  PLUS_PLUS , MINUS_MINUS , TIMES_TIMES, DIVIDE_DIVIDE,

  //Equals
  EQUAL, EQUAL_EQUAL,PLUS_EQUAL,MINUS_EQUAL, TIMES_EQUAL, DIVIDE_EQUAL, LESS_EQUAL, MORE_EQUAL,NOT_EQUAL, MOD_EQUAL,

  //Boolean
  AND,OR,XOR,NOT, GREATER, LESS, OR_OR, AND_AND,

  //Primitive
  INT,DOUBLE,CHAR,STRING,BOOL,IDEN,

  //BRACES
  OPEN_PARENTHESE,CLOSE_PARENTHESE, OPEN_BRACKET, CLOSE_BRACKET, OPEN_BRACE, CLOSE_BRACE,

  //COLONS
  SEMI_COLON, COLON, DOT, COMMA,

  END,NEW,NIL,

  //built in functions
  PRINT,PRINTLN, IMPORT, 
  VAR,VOID, RETURN,

  //control stuctures
  IF,FOR,WHILE,ELSE,STRUCT,LIST,LEN,CLASS,
  //Primitve idententifers
       IDEN_INT,IDEN_BOOL,IDEN_STRING, IDEN_DOUBLE,IDEN_CHAR,EXTENDS,LEFT_SHIFT,RIGHT_SHIFT,STATIC,
	  
};

//turn a type into a string 
static string typeToString(TokenType type){
	switch(type){
		case TokenType::PLUS:return "add";
		case TokenType::MINUS  :return "minus";
		case TokenType::TIMES  :return "times";
		case TokenType::DIVIDE  :return "divide";
		case TokenType::REMAND :return "mod";
		case TokenType::PLUS_PLUS  :return "plus_plus";
		case TokenType::MINUS_MINUS  :return "minus_minus";
		case TokenType::TIMES_TIMES :return "pow";
		case TokenType::DIVIDE_DIVIDE :return "sqrt";
		case TokenType::EQUAL :return "equal";
		case TokenType::EQUAL_EQUAL :return "equal_equal";
		case TokenType::PLUS_EQUAL :return "plus_equal";
		case TokenType::MINUS_EQUAL :return "minus_equal";
		case TokenType::TIMES_EQUAL :return "times_equal";
		case TokenType::DIVIDE_EQUAL :return "divide_equal";
		case TokenType::LESS_EQUAL :return "isle";
		case TokenType::MORE_EQUAL :return "isge";
		case TokenType::NOT_EQUAL :return "not_equal";
		case TokenType::MOD_EQUAL :return "mod_equal";
		case TokenType::AND :return "and";
		case TokenType::OR :return "or";
		case TokenType::XOR :return "xor";
		case TokenType::NOT :return "not";
		case TokenType::GREATER :return "isgt";
		case TokenType::LESS :return "islt";
		case TokenType::OR_OR :return "or_or";
		case TokenType::AND_AND :return "and_and";
		case TokenType::INT: return "int";  
		case TokenType::DOUBLE : return "double";  
		case TokenType::CHAR : return "char";  
		case TokenType::STRING : return "string";  
		case TokenType::BOOL : return "bool";  
		case TokenType::IDEN : return "iden";  
		case TokenType::OPEN_PARENTHESE : return "open_parenthese";  
		case TokenType::CLOSE_PARENTHESE : return "close_parenthese";  
		case TokenType::OPEN_BRACKET : return "open_bracket";  
		case TokenType::CLOSE_BRACKET : return "close_bracket";  
		case TokenType::OPEN_BRACE : return "open_brace";  
		case TokenType::CLOSE_BRACE : return "close_brace";  
		case TokenType::SEMI_COLON : return "semi_colon";  
		case TokenType::COLON : return "colon";  
		case TokenType::DOT : return "dot";  
		case TokenType::COMMA : return "comma";  
		case TokenType::END : return "end";  
		case TokenType::PRINT : return "print";  
		case TokenType::PRINTLN : return "println";  
		case TokenType::IMPORT : return "import";  
		case TokenType::VAR : return "var";  
		case TokenType::VOID : return "void";  
		case TokenType::RETURN : return "return";  
		case TokenType::IF : return "if";  
		case TokenType::FOR : return "for";  
		case TokenType::WHILE : return "while";  
		case TokenType::IDEN_INT : return "iden_int";  
		case TokenType::IDEN_BOOL : return "iden_bool";  
		case TokenType::IDEN_STRING : return "iden_string";  
		case TokenType::IDEN_DOUBLE : return "iden_double";  
		case TokenType::ELSE : return "else";  
		case TokenType::NEW : return "new";  
		case TokenType::LEN: return "len";
		case TokenType::CLASS: return "class";
		case TokenType::LEFT_SHIFT: return "left_shift";
		case TokenType::RIGHT_SHIFT: return "right_shift";
		case TokenType::NIL: return "NIL";
		case TokenType::STATIC: return "static";
		case TokenType::EXTENDS: return "extends";
	}
	return "";
}
#endif //ABLAZE_TOKENTYPE_H

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

  END,NEW,

  //built in functions
  PRINT,PRINTLN, IMPORT, 
  VAR,VOID, RETURN,

  //control stuctures
  IF,FOR,WHILE,ELSE,STRUCT,LIST,LEN,

  //Primitve idententifers
       IDEN_INT,IDEN_BOOL,IDEN_STRING, IDEN_DOUBLE,IDEN_CHAR,
	  
};
#endif //ABLAZE_TOKENTYPE_H

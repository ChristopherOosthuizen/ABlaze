//
// Created by chris on 12/20/20.
// The specific types of a Token
// ex: '+' is PLUS
// ex: '&' is a and
// ex: '+=' is a PLUS_EQUAL
//

#ifndef ABLAZE_TOKENTYPE_H
#define ABLAZE_TOKENTYPE_H

enum class TokenType{
  // Operators
  PLUS , MINUS , TIMES , DIVIDE ,

  // DOUBLE Operators
  PLUS_PLUS , MINUS_MINUS , TIMES_TIMES, DIVIDE_DIVIDE,

  //Equals
  EQUAL, EQUAL_EQUAL,PLUS_EQUAL,MINUS_EQUAL, TIMES_EQUAL, DIVIDE_EQUAL, LESS_EQUAL, MORE_EQUAL,NOT_EQUAL,

  //Boolean
  AND,OR,XOR,NOT, GREATER, LESS, OR_OR, AND_AND,

  //Primitive
  INT,DOUBLE,CHAR,STRING,BOOL,IDEN,

  //BRACES
  OPEN_PARENTHESE,CLOSE_PARENTHESE, OPEN_BRACKET, CLOSE_BRACKET, OPEN_BRACE, CLOSE_BRACE,

  //COLONS
  SEMI_COLON, COLON, DOT, COMMA,

  END,



};
#endif //ABLAZE_TOKENTYPE_H

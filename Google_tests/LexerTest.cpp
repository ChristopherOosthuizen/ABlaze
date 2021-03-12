// Created by chris on 12/21/20.  Test the Lexer class in its entirety a
// and make sure that it can handle any input with out
// unintentionally crashing.
//

#include "gtest/gtest.h"
#include "Lexer.h"
#include  "ErrorThrower.h"
#include <map>

/*
 * checks to see weather the tokens
 * in actual have the same type as
 * the type in expected
 */
bool haveSameTypes(vector<Token*> actual, vector<TokenType> expected){

    if(actual.size() != expected.size())
        return false;

    for(int i=0; i< actual.size();i++) {
        if(actual.at(i)->m_type != expected.at(i))
            return false;
    }
    return true;
}

/*
 * Test weather the the lexer can Handle basic operators
 * such as +,-,*,/
 */
TEST(Lexer,Operators){
    map<string,vector<TokenType>> tests;
    tests["+-*/"]= {TokenType::PLUS,TokenType::MINUS,TokenType::TIMES,TokenType::DIVIDE};

    for(auto it = tests.begin(); it != tests.end(); ++it){
        Lexer lexer(it->first);
        vector<Token*> actual = lexer.readAllTokens();
        ASSERT_TRUE(haveSameTypes(actual,it->second));
    }


}

/*
 * This test is desgned to test any one character tokens
 *
 */ 
TEST(Lexer,characters){
	string str  = "%+-*/&|:;<>{}[](),.! =";
	Lexer lexer(str);
	ASSERT_EQ(lexer.next()->m_type,TokenType::REMAND);
	ASSERT_EQ(lexer.next()->m_type,TokenType::PLUS); 
	ASSERT_EQ(lexer.next()->m_type,TokenType::MINUS);
	ASSERT_EQ(lexer.next()->m_type,TokenType::TIMES);
	ASSERT_EQ(lexer.next()->m_type,TokenType::DIVIDE);
	ASSERT_EQ(lexer.next()->m_type,TokenType::AND);
	ASSERT_EQ(lexer.next()->m_type,TokenType::OR);
	ASSERT_EQ(lexer.next()->m_type,TokenType::COLON);
	ASSERT_EQ(lexer.next()->m_type,TokenType::SEMI_COLON);
	ASSERT_EQ(lexer.next()->m_type,TokenType::LESS);
	ASSERT_EQ(lexer.next()->m_type,TokenType::GREATER);
	ASSERT_EQ(lexer.next()->m_type,TokenType::OPEN_BRACE);
	ASSERT_EQ(lexer.next()->m_type,TokenType::CLOSE_BRACE);
	ASSERT_EQ(lexer.next()->m_type,TokenType::OPEN_BRACKET);
	ASSERT_EQ(lexer.next()->m_type,TokenType::CLOSE_BRACKET);
	ASSERT_EQ(lexer.next()->m_type,TokenType::OPEN_PARENTHESE);
	ASSERT_EQ(lexer.next()->m_type,TokenType::CLOSE_PARENTHESE);
	ASSERT_EQ(lexer.next()->m_type,TokenType::COMMA);
	ASSERT_EQ(lexer.next()->m_type,TokenType::DOT);
	ASSERT_EQ(lexer.next()->m_type,TokenType::NOT);
	ASSERT_EQ(lexer.next()->m_type,TokenType::EQUAL);
	
	
}

/*
 * Two line characters 
 * This test that the program can handle all two line character such as +=, -= *&
 */
TEST(Lexer,Two){
	string str = "==+=-=*=/=!=++--//**&&||<=>=";
	Lexer lexer(str);
	ASSERT_EQ(lexer.next()->m_type,TokenType::EQUAL_EQUAL);
	ASSERT_EQ(lexer.next()->m_type,TokenType::PLUS_EQUAL);
	ASSERT_EQ(lexer.next()->m_type,TokenType::MINUS_EQUAL);
	ASSERT_EQ(lexer.next()->m_type,TokenType::TIMES_EQUAL);
	ASSERT_EQ(lexer.next()->m_type,TokenType::DIVIDE_EQUAL);
	ASSERT_EQ(lexer.next()->m_type,TokenType::NOT_EQUAL);
	ASSERT_EQ(lexer.next()->m_type,TokenType::PLUS_PLUS);
	ASSERT_EQ(lexer.next()->m_type,TokenType::MINUS_MINUS);
	ASSERT_EQ(lexer.next()->m_type,TokenType::DIVIDE_DIVIDE);
	ASSERT_EQ(lexer.next()->m_type,TokenType::TIMES_TIMES);
	ASSERT_EQ(lexer.next()->m_type,TokenType::AND_AND);
	ASSERT_EQ(lexer.next()->m_type,TokenType::OR_OR);
	ASSERT_EQ(lexer.next()->m_type,TokenType::LESS_EQUAL);
	ASSERT_EQ(lexer.next()->m_type,TokenType::MORE_EQUAL);
	
}

/*
 * Test weather a invalid character adds to the error stack
 */

TEST(Lexer,invalidCharacters){
	string  str = "@~";
	Lexer lexer(str);
	lexer.readAllTokens();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(2,ErrorThrower::errors->size());

	ErrorThrower::hasError =false;
	delete ErrorThrower::errors ;
	ErrorThrower::errors = new vector<string>();
}

/*
 * Test weather the lexer can take 
 * Strings and weather at all 
 */
TEST(String,basic){
	string str= "\"hello ther how are you\"";
	Lexer lexer(str);
	Token* token = lexer.next();
	ASSERT_EQ(token->m_type, TokenType::STRING);
	ASSERT_EQ(token->m_symbol, "hello ther how are you");
}

/*
 * This Tests weather the string function can handle 
 * \\" without breaking
 */
TEST(String,excape){
	string str= "\"hello ther \\\"how are you\"";
	Lexer lexer(str);
	Token* token = lexer.next();
	ASSERT_EQ(token->m_type, TokenType::STRING);
	ASSERT_EQ(token->m_symbol, "hello ther \\\"how are you");

}


/*
 * This test is designed to check if the 
 * compiler can accurately distinguish ints and not confuse them for doubles
 */
TEST(Inter,basic){
	string str = "12 345 790 5678 5764589 8 2 3848";
	Lexer lexer(str);
	Token* token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::INT);
	ASSERT_EQ(token->m_symbol,"12");
	delete token;
	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::INT);
	ASSERT_EQ(token->m_symbol,"345");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::INT);
	ASSERT_EQ(token->m_symbol,"790");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::INT);
	ASSERT_EQ(token->m_symbol,"5678");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::INT);
	ASSERT_EQ(token->m_symbol,"5764589");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::INT);
	ASSERT_EQ(token->m_symbol,"8");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::INT);
	ASSERT_EQ(token->m_symbol,"2");
	delete token;


	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::INT);
	ASSERT_EQ(token->m_symbol,"3848");
	delete token;

}


/*
 * This is to test weather the number function
 * can handle doubles
 */
TEST(Double,basic){
	string str = "12.0 0.345 7.90 .5678 57.64589 8.63434 2.0 3848.1";
	Lexer lexer(str);
	Token* token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::DOUBLE);
	ASSERT_EQ(token->m_symbol,"12.0");
	delete token;
	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::DOUBLE);
	ASSERT_EQ(token->m_symbol,"0.345");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::DOUBLE);
	ASSERT_EQ(token->m_symbol,"7.90");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::DOUBLE);
	ASSERT_EQ(token->m_symbol,".5678");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::DOUBLE);
	ASSERT_EQ(token->m_symbol,"57.64589");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::DOUBLE);
	ASSERT_EQ(token->m_symbol,"8.63434");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::DOUBLE);
	ASSERT_EQ(token->m_symbol,"2.0");
	delete token;


	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::DOUBLE);
	ASSERT_EQ(token->m_symbol,"3848.1");
	delete token;

}

/*
 * Test just to see if the Lexer can detect basic 
 * characters 
 */

TEST(IDEN,basic){
	Lexer lexer("hello there great nice right vars like");
	
	Token* token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN);
	ASSERT_EQ(token->m_symbol,"hello");
	delete token;


	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN);
	ASSERT_EQ(token->m_symbol,"there");
	delete token;


	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN);
	ASSERT_EQ(token->m_symbol,"great");
	delete token;


	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN);
	ASSERT_EQ(token->m_symbol,"nice");
	delete token;


	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN);
	ASSERT_EQ(token->m_symbol,"right");
	delete token;


	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN);
	ASSERT_EQ(token->m_symbol,"vars");
	delete token;


	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN);
	ASSERT_EQ(token->m_symbol,"like");
	delete token;


}

/*
 * TEST to make sure that all the 
 * Reserved keywords work and are identified correctly
 */

TEST(IDEN,reserved){
	Lexer lexer("print println import var if for while return int double string bool else char extends ");
	
	Token* token;


	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::PRINT);
	ASSERT_EQ(token->m_symbol,"print");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::PRINTLN);
	ASSERT_EQ(token->m_symbol,"println");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IMPORT);
	ASSERT_EQ(token->m_symbol,"import");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::VAR);
	ASSERT_EQ(token->m_symbol,"var");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IF);
	ASSERT_EQ(token->m_symbol,"if");
	delete token;


	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::FOR);
	ASSERT_EQ(token->m_symbol,"for");
	delete token;


	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::WHILE);
	ASSERT_EQ(token->m_symbol,"while");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::RETURN);
	ASSERT_EQ(token->m_symbol,"return");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN_INT);
	ASSERT_EQ(token->m_symbol,"int");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN_DOUBLE);
	ASSERT_EQ(token->m_symbol,"double");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN_STRING);
	ASSERT_EQ(token->m_symbol,"string");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN_BOOL);
	ASSERT_EQ(token->m_symbol,"bool");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::ELSE);
	ASSERT_EQ(token->m_symbol,"else");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN_CHAR);
	ASSERT_EQ(token->m_symbol,"char");
	delete token;

	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::EXTENDS);
	ASSERT_EQ(token->m_symbol,"extends");
	delete token;



}

/*
 * Make sure the lexer throws a error
 * when there is a double with two dots
 */
TEST(ERRORS,doubleDot){
	Lexer lexer("8..9");
	lexer.next();
	ASSERT_TRUE(ErrorThrower::hasError);
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
}

/*
 * Throw a error on unterminated string 
 */

TEST(ERRORS, unterminated){
	Lexer lexer("\" hello there ");
	lexer.next();
	ASSERT_TRUE(ErrorThrower::hasError);
	ErrorThrower::hasError = false; 
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
}

TEST(Lexer,chars){
        Lexer lexer("'3'");
        Token* token;
        token = lexer.next();
        ASSERT_EQ(token->m_type,TokenType::CHAR);
}


//Test weather the parser can handle strings properly
TEST(ASTSTRUT, mores){
        Lexer lexer("for(var i=0; i != 2; ++i){ printer(\"hello\");}");
        Token* token; 

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::FOR);
        delete token;	  


       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::OPEN_PARENTHESE);
        delete token;	  

       
       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::VAR);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::EQUAL);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::INT);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::SEMI_COLON);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::NOT_EQUAL);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::INT);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::SEMI_COLON);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::PLUS_PLUS);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::CLOSE_PARENTHESE);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::OPEN_BRACE);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::IDEN);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::OPEN_PARENTHESE);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::STRING);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::CLOSE_PARENTHESE);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::SEMI_COLON);
        delete token;	  

       	token = lexer.next();
	ASSERT_EQ(token->m_type,TokenType::CLOSE_BRACE);
        delete token;	  


}


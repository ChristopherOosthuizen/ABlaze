// Created by chris on 12/21/20.  Test the Lexer class in its entirety a
// and make sure that it can handle any input with out
// unintentionally crashing.
//

#include "gtest/gtest.h"
#include "Lexer.h"
#include <map>

/*
 * checks to see weather the tokens
 * in actual have the same type as
 * the type in expected
 */
bool haveSameTypes(vector<Token> actual, vector<TokenType> expected){

    if(actual.size() != expected.size())
        return false;

    for(int i=0; i< actual.size();i++) {
        if(actual.at(i).m_type != expected.at(i))
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
        vector<Token> actual = lexer.readAllTokens();
        ASSERT_TRUE(haveSameTypes(actual,it->second));
    }


}

/*
 * This test is desgned to test any one character tokens
 *
 */ 
TEST(Lexer,characters){
	string str  = "+-*/&|:;<>{}[](),.! =";
	Lexer lexer(str);
	ASSERT_EQ(lexer.next().m_type,TokenType::PLUS); 
	ASSERT_EQ(lexer.next().m_type,TokenType::MINUS);
	ASSERT_EQ(lexer.next().m_type,TokenType::TIMES);
	ASSERT_EQ(lexer.next().m_type,TokenType::DIVIDE);
	ASSERT_EQ(lexer.next().m_type,TokenType::AND);
	ASSERT_EQ(lexer.next().m_type,TokenType::OR);
	ASSERT_EQ(lexer.next().m_type,TokenType::COLON);
	ASSERT_EQ(lexer.next().m_type,TokenType::SEMI_COLON);
	ASSERT_EQ(lexer.next().m_type,TokenType::LESS);
	ASSERT_EQ(lexer.next().m_type,TokenType::GREATER);
	ASSERT_EQ(lexer.next().m_type,TokenType::OPEN_BRACE);
	ASSERT_EQ(lexer.next().m_type,TokenType::CLOSE_BRACE);
	ASSERT_EQ(lexer.next().m_type,TokenType::OPEN_BRACKET);
	ASSERT_EQ(lexer.next().m_type,TokenType::CLOSE_BRACKET);
	ASSERT_EQ(lexer.next().m_type,TokenType::OPEN_PARENTHESE);
	ASSERT_EQ(lexer.next().m_type,TokenType::CLOSE_PARENTHESE);
	ASSERT_EQ(lexer.next().m_type,TokenType::COMMA);
	ASSERT_EQ(lexer.next().m_type,TokenType::DOT);
	ASSERT_EQ(lexer.next().m_type,TokenType::NOT);
	ASSERT_EQ(lexer.next().m_type,TokenType::EQUAL);
	
	
}

/*
 * Two line characters 
 * This test that the program can handle all two line character such as +=, -= *&
 */
TEST(Lexer,Two){
	string str = "==+=-=*=/=!=++--//**&&||<=>=";
	Lexer lexer(str);
	ASSERT_EQ(lexer.next().m_type,TokenType::EQUAL_EQUAL);
	ASSERT_EQ(lexer.next().m_type,TokenType::PLUS_EQUAL);
	ASSERT_EQ(lexer.next().m_type,TokenType::MINUS_EQUAL);
	ASSERT_EQ(lexer.next().m_type,TokenType::TIMES_EQUAL);
	ASSERT_EQ(lexer.next().m_type,TokenType::DIVIDE_EQUAL);
	ASSERT_EQ(lexer.next().m_type,TokenType::NOT_EQUAL);
	ASSERT_EQ(lexer.next().m_type,TokenType::PLUS_PLUS);
	ASSERT_EQ(lexer.next().m_type,TokenType::MINUS_MINUS);
	ASSERT_EQ(lexer.next().m_type,TokenType::DIVIDE_DIVIDE);
	ASSERT_EQ(lexer.next().m_type,TokenType::TIMES_TIMES);
	ASSERT_EQ(lexer.next().m_type,TokenType::AND_AND);
	ASSERT_EQ(lexer.next().m_type,TokenType::OR_OR);
	ASSERT_EQ(lexer.next().m_type,TokenType::LESS_EQUAL);
	ASSERT_EQ(lexer.next().m_type,TokenType::MORE_EQUAL);
	
}

/*
 * Test weather the lexer can take 
 * Strings and weather at all 
 */
TEST(String,basic){
	string str= "\"hello ther how are you\"";
	Lexer lexer(str);
	Token token = lexer.next();
	ASSERT_EQ(token.m_type, TokenType::STRING);
	ASSERT_EQ(token.m_symbol, "hello ther how are you");
}

//
// Created by chris on 12/21/20.
// Test the Lexer class in its entirety a
// and make sure that it can handle any input with out
// unintentionally crashing.
//

#include "gtest/gtest.h"
#include "../Lexer/Lexer.h"
#include "../Lexer/Lexer.cpp"
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
// Created by Christopher Oosthuizen on 01/27/21
// General testing for the AST Tree


#include "gtest/gtest.h"
#include "AST.h"
#include "ASTGen.h"
#include "Lexer.h"
#include "ErrorThrower.h"

TEST(ASTGeneral,mainFunction){
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
        string end = "int main(){\nint i = 12;\nprint(i);\ni++;\n}";
        Lexer lexer(end);
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");
        ASSERT_TRUE(((Body*)body->m_lines->at(0))->m_control!= nullptr);
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_control->name(),"Function");
        Body* second =((Body*)body->m_lines->at(0)); 
        ASSERT_EQ(second->m_lines->size(),3);
        ASSERT_EQ(second->m_lines->at(0)->name(),"Decleration"); 
        ASSERT_EQ(second->m_lines->at(1)->name(),"FunctionCall"); 
        ASSERT_EQ(second->m_lines->at(2)->name(),"UnOP"); 


}

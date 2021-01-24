/* Created by Christopher Oosthuizen on 01/18/21
 * This class tests the ASTGen spicifically the Bodys 
 */

#include "gtest/gtest.h"
#include "Lexer.h"
#include "ASTGen.h"

/*
 * Test weather the generator can 
 * properly handle if statments
 */
TEST(ASTSTRUT,ifs){ 
        Lexer lexer("if(true){ var i = 12; ++i; }");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);
        Expression* expr = body->m_lines->at(0);
        ASSERT_TRUE(expr !=nullptr);
        ASSERT_EQ(expr->name(),"Body");
        Body* bod = (Body*)expr;
        ASSERT_EQ(bod->m_lines->size(),2);
        ASSERT_EQ(bod->m_control->name(),"If");
}

//Test weather the ASTGen can properly handle for statments

TEST(ASTSTRUT,whiles){
        Lexer lexer("while(true){var i = 12+9*3; right(i); ++left;}");        
        ASTGen gen (lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_TRUE(body->m_lines->at(0) !=nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");
        body= (Body*)body->m_lines->at(0);
        ASSERT_EQ(body->m_control->name(),"While");
        ASSERT_EQ(body->m_lines->size(),3);
}

TEST(ASTSTRUT, fors){
        Lexer lexer("for(var i=0; i != 2; ++i){ printer(\"hello\");}");
        ASTGen gen (lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_TRUE(body->m_lines->at(0) !=nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");
        body= (Body*)body->m_lines->at(0);
        ASSERT_EQ(body->m_control->name(),"For");
        ASSERT_EQ(body->m_lines->size(),1);

}

// Test weather the ASTGen can handle functions
TEST(ASTSTRUT, function){
        Lexer lexer("var hello(var right, var left){ printer(); var like = 12; }");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_control->name(),"Function");
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_lines->size(),2); 
}

// Test weather the ASTGen can handle void functions
TEST(ASTSTRUT, voids){
        Lexer lexer("void hello(var right, var left){ printer(); var like = 12; }");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_control->name(),"Function");
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_lines->size(),2); 
}

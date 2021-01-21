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

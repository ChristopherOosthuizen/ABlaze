#include "gtest/gtest.h"
#include "AST.h"
#include "Lexer.h"
#include "ASTGen.h"
#include <typeinfo>


/*
 * Test basic AST construction of just using the
 * basics
 */
TEST(ASTbasic,operations){
	Lexer lexer("5+8*2;");
	ASTGen astgen(lexer.readAllTokens());
	BinOP* expr = (BinOP*)astgen.generateAST()->m_lines->at(0);
	Token* token = expr->m_op;
	ASSERT_EQ(token->m_type,TokenType::PLUS);
        ASSERT_TRUE(expr->m_right !=nullptr);
	ASSERT_EQ(((BinOP*)expr->m_right)->m_op->m_type,TokenType::TIMES);
	delete expr;


}

/*
 * A slightly more in depth test to see
 * weather times and add are handled properly
 */
TEST(ASTbasic,pemdas){
	Lexer lexer("5*8+2;");
	ASTGen astgen(lexer.readAllTokens());
        Body* body = astgen.generateAST();
	BinOP* expr = (BinOP*)body->m_lines->at(0);
	Token* token = expr->m_op;
	ASSERT_EQ(token->m_type,TokenType::PLUS);
	ASSERT_EQ(((BinOP*)expr->m_left)->m_op->m_type,TokenType::TIMES);

        delete  body;
}


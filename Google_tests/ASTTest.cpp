// Created by Christopher Oosthuizen on 01/11/21
// The point of this class is to test AST construction 
// and functiality


#include "gtest/gtest.h"
#include "AST.h"
#include "Lexer.h"
#include "ASTGen.h"

/*
 * Test basic AST consturction of just using the 
 * basics 
 */
TEST(ASTbasic,operations){
	Lexer lexer("5+8*2");
	ASTGen astgen(lexer.readAllTokens());
	BinOP* expr = (BinOP*)astgen.generateAST();	
	Token* token = expr->m_op;
	ASSERT_EQ(token->m_type,TokenType::PLUS);
	ASSERT_EQ(((BinOP*)expr->m_right)->m_op->m_type,TokenType::TIMES);

	delete expr;


}

/*
 * A slightly more in depth test to see
 * weather times and add are hadled properly
 */
TEST(ASTbasic,pemdas){
	Lexer lexer("5*8+2");
	ASTGen astgen(lexer.readAllTokens());
	BinOP* expr = (BinOP*)astgen.generateAST();	
	Token* token = expr->m_op;
	ASSERT_EQ(token->m_type,TokenType::PLUS);
	ASSERT_EQ(((BinOP*)expr->m_left)->m_op->m_type,TokenType::TIMES);

	delete expr;


}


/*
 * Test to see if the ASTGen can construct ASTS
 * based of equations with no parans
 */
TEST(ASTGen,equations){
	Lexer* lexer = new Lexer("6+6*2 ");
    ASTGen* gen = new ASTGen(lexer->readAllTokens());
    BinOP* expr = (BinOP*)gen->generateAST(); 
    ASSERT_EQ(expr->m_op->m_type, TokenType::PLUS);
    ASSERT_EQ(((BinOP*)expr->m_right)->m_op->m_type,TokenType::TIMES);
    delete lexer;
    delete gen;
    delete expr;

    Lexer* lex = new Lexer("6*8+12*9+10/2-6 ");
    ASTGen* gene = new ASTGen(lex->readAllTokens());
    BinOP* exp = (BinOP*)gene->generateAST(); 
    ASSERT_EQ(exp->m_op->m_type, TokenType::PLUS);
    ASSERT_EQ(((BinOP*)exp->m_left)->m_op->m_type,TokenType::TIMES);
    BinOP* right = (BinOP*)exp->m_right;
    ASSERT_EQ(right->m_op->m_type,TokenType::PLUS);
    ASSERT_EQ(  ((BinOP*)right->m_left)->m_op->m_type,TokenType::TIMES);
    ASSERT_EQ(((BinOP*)right->m_right)->m_op->m_type,TokenType::MINUS);
    ASSERT_EQ(((BinOP*)((BinOP*)right->m_right)->m_left)->m_op->m_type,TokenType::DIVIDE);

    delete lexer;
    delete gen;
    delete expr;
}

/*
 * testing weather the ASTGen can handle paretheses
 */
 TEST(ASTGen,Paretheses){
	Lexer* lexer = new Lexer("(6+6)*2 ");
    ASTGen* gen = new ASTGen(lexer->readAllTokens());
    BinOP* expr = (BinOP*)gen->generateAST(); 
    ASSERT_EQ(expr->m_op->m_type, TokenType::PLUS);
    ASSERT_EQ(((BinOP*)expr->m_right)->m_op->m_type,TokenType::TIMES);
    delete lexer;
    delete gen;
    delete expr;

    
 }


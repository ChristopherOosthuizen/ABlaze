// Created by Christopher Oosthuizen on 01/11/21
// The point of this class is to test AST construction
// and functionality


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
	BinOP* expr = (BinOP*)astgen.generateAST()->m_lines->at(0);
	Token* token = expr->m_op;
	ASSERT_EQ(token->m_type,TokenType::PLUS);
	ASSERT_EQ(((BinOP*)expr->m_left)->m_op->m_type,TokenType::TIMES);

}


/*
 * Test to see if the ASTGen can construct ASTS
 * based of equations with no parameters
 */
TEST(ASTGen,equations){

        Lexer lex("6*8+12*9+10/2-6 ;");
        ASTGen gene(lex.readAllTokens());
        BinOP* exp = (BinOP*)gene.generateAST()->m_lines->at(0);
        ASSERT_EQ(exp->m_op->m_type, TokenType::PLUS);
        ASSERT_EQ(((BinOP*)exp->m_left)->m_op->m_type,TokenType::TIMES);
        BinOP* right = (BinOP*)exp->m_right;
        ASSERT_EQ(right->m_op->m_type,TokenType::PLUS);
        ASSERT_EQ(  ((BinOP*)right->m_left)->m_op->m_type,TokenType::TIMES);
        ASSERT_EQ(((BinOP*)right->m_right)->m_op->m_type,TokenType::MINUS);
        ASSERT_EQ(((BinOP*)((BinOP*)right->m_right)->m_left)->m_op->m_type,TokenType::DIVIDE);

}

/*
 * testing weather the ASTGen can handle parentheses
 *
 */
 TEST(ASTGen,Paretheses){
        Lexer lexer("(6+6)*2 ;");
        ASTGen gen (lexer.readAllTokens());
        BinOP* expr = (BinOP*)gen.generateAST()->m_lines->at(0);
        ASSERT_TRUE(expr!=nullptr);
        ASSERT_EQ(expr->m_op->m_type, TokenType::TIMES);

        ASSERT_TRUE(expr->m_left !=nullptr);
        ASSERT_EQ(((BinOP*)expr->m_left)->m_op->m_type,TokenType::PLUS);


 }

/*
 * Test to see if declaration work
 *
 */
TEST(ASTGen, Decleration){
        Lexer lexer("var hello = right;");
        ASTGen gen(lexer.readAllTokens());
        Decleration* dec = (Decleration*)gen.generateAST()->m_lines->at(0);
        ASSERT_TRUE(dec !=nullptr);
        ASSERT_EQ(dec->m_name->m_token->m_symbol,"hello");
        ASSERT_TRUE(dec->m_value !=nullptr);
        ASSERT_EQ(((Literal*)dec->m_value)->m_token->m_symbol,"right");

}
//Test multiple delclerations
TEST(ASTGen, DecMulti){
        Lexer lexer("var i= 12+9; var notT = i != 12; ++i; i = 6;");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),4);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Decleration");
        ASSERT_TRUE(body->m_lines->at(1) != nullptr);
        ASSERT_EQ(body->m_lines->at(1)->name(),"Decleration");
        ASSERT_TRUE(body->m_lines->at(2) != nullptr);
        ASSERT_EQ(body->m_lines->at(2)->name(),"UnOP");
        ASSERT_TRUE(body->m_lines->at(3) != nullptr);
        ASSERT_EQ(body->m_lines->at(3)->name(),"Decleration");


}

/*
 * Test weather you can call a function without it breaking
 */
TEST(ASTGen, functionscall){
        Lexer lexer("hello();");
        ASTGen gen(lexer.readAllTokens());
        Body* bas = gen.generateAST();
        ASSERT_TRUE(bas != nullptr);
        Expression* expr = (Expression*)bas->m_lines->at(0);
        ASSERT_TRUE(expr !=NULL);
        ASSERT_EQ(expr->name(),"FunctionCall");
        FunctionCall* call = (FunctionCall*) expr;
        ASSERT_EQ(call->m_name->m_token->m_symbol,"hello");
        ASSERT_EQ(call->m_args->size(),0);

}

/*
 * Test weather function call can handle one paramter
 */
TEST(ASTGen, functionOne){
        Lexer lexer("func(12);");
        ASTGen gen(lexer.readAllTokens());
        Expression* expr = (Expression*)gen.generateAST()->m_lines->at(0);
        ASSERT_EQ(expr->name(),"FunctionCall");
        FunctionCall* call = (FunctionCall*)  expr;
        ASSERT_EQ(call->m_name->m_token->m_symbol,"func");
        ASSERT_EQ(call->m_args->size(),1);
        ASSERT_TRUE(call->m_args->at(0) != nullptr);
        ASSERT_EQ(((Literal*)call->m_args->at(0))->m_token->m_symbol,"12");

}

/*
 * Test weather a call can handle multiple
 * paramaters
 */
TEST(ASTGen ,patamtersMultiple){
        Lexer lexer("function(12,14,right, 12+4);");
        ASTGen gen(lexer.readAllTokens());
        Expression* expr = (Expression*)gen.generateAST()->m_lines->at(0);
        ASSERT_TRUE(expr != nullptr);
        ASSERT_EQ(expr->name(),"FunctionCall");
        FunctionCall* call = (FunctionCall*)expr;
        ASSERT_EQ(call->m_name->m_token->m_symbol,"function");
        ASSERT_EQ(call->m_args->size(),4);
        ASSERT_TRUE(call->m_args->at(0) != nullptr);
        ASSERT_EQ(((Literal*)call->m_args->at(0))->m_token->m_symbol,"12");

        ASSERT_TRUE(call->m_args->at(1) != nullptr);
        ASSERT_EQ(((Literal*)call->m_args->at(1))->m_token->m_symbol,"14");

        ASSERT_TRUE(call->m_args->at(2) != nullptr);
        ASSERT_EQ(((Literal*)call->m_args->at(2))->m_token->m_symbol,"right");

        ASSERT_TRUE(call->m_args->at(3) != nullptr);
        ASSERT_EQ(call->m_args->at(3)->name(),"BinOP");
        ASSERT_EQ(((BinOP*)call->m_args->at(3))->m_op->m_type,TokenType::PLUS);



}

/*
 * Test weather a function call can be reconginzed within a binaryOP
 */
TEST(ASTGen , functionWithin){
        Lexer lexer("6+func(12);");
        ASTGen gen(lexer.readAllTokens());
        Expression* expr = (Expression*)gen.generateAST()->m_lines->at(0);
        ASSERT_TRUE(expr != nullptr);
        ASSERT_EQ(expr->name(),"BinOP");
        BinOP* op = (BinOP*) expr;
        ASSERT_EQ(op->m_op->m_type,TokenType::PLUS);
        ASSERT_TRUE(op->m_left != nullptr);
        ASSERT_EQ(op->m_left->name(), "Literal");
        ASSERT_TRUE(op->m_right !=nullptr);
        ASSERT_EQ(op->m_right->name() ,"FunctionCall");
        FunctionCall* call = (FunctionCall*)op->m_right;
        ASSERT_EQ(call->m_name->m_token->m_symbol ,"func");
        ASSERT_EQ(call->m_args->size(),1);
}

// Test basic boolean functions of the AST tree
TEST(Boolean, basic){
        Lexer lexer("right == left || true && left != right;");
        ASTGen gen (lexer.readAllTokens());
        Expression* expr = (Expression*)gen.generateAST()->m_lines->at(0);
        ASSERT_TRUE(expr !=nullptr);
        ASSERT_EQ(expr->name(), "BinOP");
        BinOP* op = (BinOP*) expr;
        ASSERT_EQ(op->m_op->m_type,TokenType::AND_AND);
        ASSERT_TRUE(op->m_left !=nullptr);
        ASSERT_EQ(op->m_left->name(),"BinOP");
        BinOP* left= (BinOP*)op->m_left;
        ASSERT_EQ(left->m_op->m_type,TokenType::OR_OR);
        ASSERT_TRUE(left->m_right !=nullptr);
        ASSERT_EQ(left->m_right->name(),"Literal");
        ASSERT_TRUE(left->m_left !=nullptr);
        ASSERT_EQ(left->m_left->name(),"BinOP");
        ASSERT_EQ(((BinOP*)left->m_left)->m_op->m_type,TokenType::EQUAL_EQUAL);
        ASSERT_TRUE(op->m_right !=nullptr);
        ASSERT_EQ(op->m_right->name(),"BinOP");
        ASSERT_EQ(((BinOP*)op->m_right)->m_op->m_type,TokenType::NOT_EQUAL);

}

//Test weather the ASTGen can handle <> <= >=
TEST(Boolean, comparison){
        Lexer lexer("var bools = 12< 14;");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_TRUE(body->m_lines->at(0) !=nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Decleration");
        ASSERT_EQ(((Decleration*)body->m_lines->at(0))->m_value->name(),"BinOP");
}

//Unary operators
TEST(Unary, basic){
        Lexer lexer("++lets; --lets; !bool;");
        ASTGen gen (lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),3);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"UnOP");
        ASSERT_TRUE(body->m_lines->at(1) != nullptr);
        ASSERT_EQ(body->m_lines->at(1)->name(),"UnOP");
        ASSERT_TRUE(body->m_lines->at(2) != nullptr);
        ASSERT_EQ(body->m_lines->at(2)->name(),"UnOP");

}

//Test weather the ast parser can handle ** and // which represent power
TEST(Power, basic){
        Lexer lexer("5*2**2//3+4;");
        ASTGen gen(lexer.readAllTokens());
        Expression* expression = (Expression*) gen.generateAST()->m_lines->at(0);
        ASSERT_TRUE(expression != nullptr);
        ASSERT_EQ(expression->name(),"BinOP");
        BinOP* op = (BinOP*)expression;
        ASSERT_EQ(op->m_op->m_type,TokenType::PLUS);
        ASSERT_TRUE(op->m_left !=nullptr);
        ASSERT_EQ(op->m_left->name(),"BinOP");
        op = (BinOP*)op->m_left;
        ASSERT_EQ(op->m_op->m_type,TokenType::DIVIDE_DIVIDE);
        ASSERT_TRUE(op->m_left !=nullptr);
        ASSERT_EQ(op->m_left->name(),"BinOP");
        op = (BinOP*)op->m_left;
        ASSERT_EQ(op->m_op->m_type,TokenType::TIMES_TIMES);
        ASSERT_TRUE(op->m_left!= nullptr);
        ASSERT_EQ(op->m_left->name(),"BinOP");
        op = (BinOP*)op->m_left;
        ASSERT_EQ(op->m_op->m_type,TokenType::TIMES);

}

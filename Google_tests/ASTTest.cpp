// Created by Christopher Oosthuizen on 01/11/21
// The point of this class is to test AST construction
// and functionality


#include "gtest/gtest.h"
#include "AST.h"
#include "Lexer.h"
#include "ASTGen.h"
#include <typeinfo>
/*
 * Test to see if declaration work
 *
 */
TEST(ASTGen, Decleration){
        Lexer lexer("var hello = right;");
        ASTGen gen(lexer.readAllTokens());
        Decleration* dec = (Decleration*)gen.generateAST()->m_lines->at(0);
        ASSERT_TRUE(dec !=nullptr);
        ASSERT_EQ(((Literal*)dec->m_name)->m_token->m_symbol,"hello");
        ASSERT_TRUE(dec->m_value !=nullptr);
        ASSERT_EQ(((Literal*)dec->m_value)->m_token->m_symbol,"right");

}

// Test weather the dec can work on *= +=
TEST(ASTGen, SpecilizedDec){
        Lexer lexer("i +=12 ; l *=4; lets *=14;");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),3);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Decleration");
        ASSERT_TRUE(body->m_lines->at(1) != nullptr);
        ASSERT_EQ(body->m_lines->at(1)->name(),"Decleration");
        ASSERT_TRUE(body->m_lines->at(2) != nullptr);
        ASSERT_EQ(body->m_lines->at(2)->name(),"Decleration");


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

//Unary operators
TEST(Unary, postfix){
        Lexer lexer("lets++; lets--;");
        ASTGen gen (lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),2);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"UnOP");
        ASSERT_TRUE(body->m_lines->at(1) != nullptr);
        ASSERT_EQ(body->m_lines->at(1)->name(),"UnOP");

}


//Test weather the ast parser can handle ** and // which represent power
TEST(Power, basic){
        Lexer lexer("5*2**2//3+4;");
        ASTGen gen(lexer.readAllTokens());
        Expression* expression = (Expression*) gen.generateAST()->m_lines->at(0);
        ASSERT_TRUE(expression != nullptr);
        
}

//test weather decleratiosn with spicific types works
TEST(Dec, Type){
        Lexer lexer("int i = 12; bool h = true; string hello =\"hello\"; double right = 12.0;");
        ASTGen gen (lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),4);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Decleration");
        ASSERT_TRUE(body->m_lines->at(1) != nullptr);
        ASSERT_EQ(body->m_lines->at(1)->name(),"Decleration");
        ASSERT_TRUE(body->m_lines->at(2) != nullptr);
        ASSERT_EQ(body->m_lines->at(2)->name(),"Decleration");
        ASSERT_TRUE(body->m_lines->at(3) != nullptr);
        ASSERT_EQ(body->m_lines->at(3)->name(),"Decleration");


}

TEST(Dec,structs){
        Lexer lexer("var pos = new Pos; pos.x = 12; print(pos.x);");
        ASTGen gen (lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),3);
        Decleration* dec;
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Decleration");
        dec = (Decleration*)body->m_lines->at(0);
        ASSERT_TRUE(dec->m_value != nullptr);
        ASSERT_EQ(dec->m_value->name(),"New");
        ASSERT_TRUE(body->m_lines->at(1) != nullptr);
        ASSERT_EQ(body->m_lines->at(1)->name(),"Decleration");
        dec = (Decleration*)body->m_lines->at(1);
        ASSERT_TRUE(dec->m_name != nullptr);
        ASSERT_EQ(dec->m_name->name(),"Dot");
        ASSERT_TRUE(body->m_lines->at(2) != nullptr);
        ASSERT_EQ(body->m_lines->at(2)->name(),"FunctionCall");


}

TEST(AST, dots){
        Lexer lexer("print(position.x+position.y);");
        ASTGen gen(lexer.readAllTokens());
        Expression* expression = (Expression*) gen.generateAST()->m_lines->at(0);
        ASSERT_TRUE(expression != nullptr);
        ASSERT_EQ(expression->name(),"FunctionCall");
        BinOP* op = (BinOP*)((FunctionCall*)expression)->m_args->at(0);
        ASSERT_TRUE(op->m_left !=nullptr);
        ASSERT_EQ(op->m_left->name(),"Dot");
        ASSERT_TRUE(op->m_right !=nullptr);
        ASSERT_EQ(op->m_right->name(),"Dot");
}
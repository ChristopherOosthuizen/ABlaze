/* Created by Christopher Oosthuizen on 01/18/21
 * This class tests the ASTGen spicifically the Bodys 
 */

#include "gtest/gtest.h"
#include "Lexer.h"
#include "ASTGen.h"
#include "ErrorThrower.h"

/*
 * Test weather the generator can 
 * properly handle if statments
 */
TEST(ASTSTRUT,ifs){ 
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
        Lexer lexer("if(true){ var i = 12; ++i; }");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);

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
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
        Lexer lexer("while(true){var i = 12+9*3; right(i); ++left;}");        
        ASTGen gen (lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);

        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_TRUE(body->m_lines->at(0) !=nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");
        body= (Body*)body->m_lines->at(0);
        ASSERT_EQ(body->m_control->name(),"While");
        ASSERT_EQ(body->m_lines->size(),3);
}

TEST(ASTSTRUT, fors){

        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
 

        Lexer lexer("for(var i=0; i != 2; ++i){ printer(\"hello\");}");
        ASTGen gen (lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);

        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_TRUE(body->m_lines->at(0) !=nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");
        body= (Body*)body->m_lines->at(0);
        ASSERT_EQ(body->m_control->name(),"For");
        ASSERT_EQ(body->m_lines->size(),1);

}

// Test weather the ASTGen can handle functions
TEST(ASTSTRUT, function){
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
 
        Lexer lexer("var hello(var right, var left){ printer(); var like = 12; }");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_control->name(),"Function");
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_lines->size(),2); 
}

// Test weather the ASTGen can handle void functions
TEST(ASTSTRUT, voids){
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
 
        Lexer lexer("void hello(var right, var left){ printer(); var like = 12; }");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);

        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_control->name(),"Function");
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_lines->size(),2); 
}

// Test weather return statmenets work
TEST(ASTSTRUT, returns){
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
 
        Lexer lexer("return 12;");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);

        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"BuiltIn");
        
}

//Test weather import works
TEST(ASTSTRUT, Imports){
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
 
        Lexer lexer("import \"hello\";");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"BuiltIn");
 
}

//Test whether arrays and be created by the ASTGen
TEST(ASTSTRUT, array){
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
 
        Lexer lexer("int: list = int[12];  int i = list[12];  list[i] = 12;");
        ASTGen gen(lexer.readAllTokens());
        ASSERT_TRUE(!ErrorThrower::hasError);
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),3);
        ASSERT_TRUE(body->m_lines->at(0) != nullptr);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Decleration");
        ASSERT_TRUE((((Decleration*)body->m_lines->at(0))->m_isArray));
        ASSERT_EQ((((Decleration*)body->m_lines->at(0))->m_value)->name(),"ArrayLiteral");
        ASSERT_TRUE(body->m_lines->at(1) != nullptr);
        ASSERT_EQ(body->m_lines->at(1)->name(),"Decleration");
        ASSERT_TRUE(body->m_lines->at(2) != nullptr);
        ASSERT_EQ(body->m_lines->at(2)->name(),"Decleration");
}



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

TEST(ASTGeneral,elsser){
        Lexer lexer("int i=12; int o =13; int right = i < o; if( right ){ print( i ); } else { print(o); }");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
 
        ASSERT_EQ(body->m_lines->size(),5);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Decleration");
        ASSERT_EQ(body->m_lines->at(1)->name(),"Decleration");
        ASSERT_EQ(body->m_lines->at(2)->name(),"Decleration");
        ASSERT_EQ(body->m_lines->at(3)->name(),"Body");
        ASSERT_EQ(body->m_lines->at(4)->name(),"Body");
        Body* ifs = (Body*)body->m_lines->at(3);
        ASSERT_EQ(ifs->m_lines->size(),1);
        ASSERT_EQ(ifs->m_lines->at(0)->name(),"FunctionCall");
         ifs = (Body*)body->m_lines->at(4);
        ASSERT_EQ(ifs->m_lines->size(),1);
        ASSERT_EQ(ifs->m_lines->at(0)->name(),"FunctionCall");

}

TEST(ASTGenetal,whiles){
        Lexer lexer("int i=0; while(i<12){i =i+1; print(i);} ");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),2);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Decleration");
        ASSERT_EQ(body->m_lines->at(1)->name(),"Body");
        Body* whiler= (Body*)body->m_lines->at(1);
        WhileStat* stat = (WhileStat*)whiler->m_control;
        ASSERT_EQ(stat->m_control->name(),"BinOP");
        ASSERT_EQ(whiler->m_lines->size(),2);
        ASSERT_EQ(whiler->m_lines->at(0)->name(),"Decleration");
        ASSERT_EQ(whiler->m_lines->at(1)->name(),"FunctionCall");


}

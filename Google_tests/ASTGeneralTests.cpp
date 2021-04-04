// Created by Christopher Oosthuizen on 01/27/21
// General testing for the AST Tree


#include "gtest/gtest.h"
#include "AST.h"
#include "ASTGen.h"
#include "Lexer.h"
#include "ErrorThrower.h"

TEST(ASTGeneral,prints){
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
        string end = "print(15+5);\nprint(1);\n";
        Lexer lexer(end);
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);
        ASSERT_EQ(body->m_lines->size(),2);
        ASSERT_EQ(body->m_lines->at(0)->name(),"BuiltIn");
        ASSERT_EQ(body->m_lines->at(1)->name(),"BuiltIn");

 
}

TEST(ASTGeneral,classs){
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
        string end = "class name{int i; int o; void strut(int x, int y){ i=x; o=y;} }";
        Lexer lexer(end);
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");

 
}

TEST(ASTGeneral,arrayFunc){
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
        string end = "int: its(){return {12,14};}";
        Lexer lexer(end);
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");
}



TEST(ASTGeneral,array){
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
        string end = "int i = {12,14,15,16}; ";
        Lexer lexer(end);
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Decleration");
        ASSERT_EQ(((Decleration*)body->m_lines->at(0))->m_value->name(),"Array");

 
}



TEST(ASTGeneral,Structs){
        ErrorThrower::hasError = false;
        ErrorThrower::errors = new vector<string>();
        string end = "struct name{int i; int o; }";
        Lexer lexer(end);
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(!ErrorThrower::hasError);
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_EQ(body->m_lines->at(0)->name(),"Body");

 
}


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
        ASSERT_EQ(second->m_lines->at(1)->name(),"BuiltIn"); 
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
        ASSERT_EQ(ifs->m_lines->at(0)->name(),"BuiltIn");
         ifs = (Body*)body->m_lines->at(4);
        ASSERT_EQ(ifs->m_lines->size(),1);
        ASSERT_EQ(ifs->m_lines->at(0)->name(),"BuiltIn");

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
        ASSERT_EQ(whiler->m_lines->at(1)->name(),"BuiltIn");


}

TEST(ASTGenetal,mainFunction){
        Lexer lexer("int main(){int i=12; int o =13; int right = i < o; if( right ){ print( i ); } else { print(o); }}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_lines->size(),5);

}

TEST(ASTGenetal,equals){
        Lexer lexer("int main(){if(1==1){print(\"hello\");}}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);

}

TEST(ASTGenetal,extends){
        Lexer lexer("struct Left extends Direction{int i=12;}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);

}

TEST(ASTGenetal,Static){
        Lexer lexer("struct Left {int i=12; static int make(){return 12;}}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_lines->size(),2);
}

TEST(ASTGenetal,publics){
        Lexer lexer("struct Left {int i=12; public static int make(){return 12;} public int rake(){return 15;}}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_lines->size(),3);
}


TEST(ASTGenetal,maps){
        Lexer lexer("void main(){string:int maper = string[int]; int str = maper[\"hello\"]; maper.keys(); maper.vals();}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ASSERT_EQ(body->m_lines->size(),1);
        ASSERT_EQ(((Body*)body->m_lines->at(0))->m_lines->size(),4);
}




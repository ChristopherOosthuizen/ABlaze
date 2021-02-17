#include "gtest/gtest.h"
#include "AST.h"
#include "ASTGen.h"
#include "ErrorThrower.h"
#include "Lexer.h"
#include "ByteGen.h"

TEST(ByteCode,equations){
        Lexer lexer("int i=12+8*2");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),11);
        ASSERT_EQ(strs->at(0),"push"); 
        ASSERT_EQ(strs->at(1),"12"); 
        ASSERT_EQ(strs->at(2),"push"); 
        ASSERT_EQ(strs->at(3),"8"); 
        ASSERT_EQ(strs->at(4),"push"); 
        ASSERT_EQ(strs->at(5),"2"); 
        ASSERT_EQ(strs->at(6),"times");
        ASSERT_EQ(strs->at(7),"add"); 
        ASSERT_EQ(strs->at(8),"store"); 
        ASSERT_EQ(strs->at(9),"i"); 
        ASSERT_EQ(strs->at(10),"halt"); 

}


TEST(ByteCode,equationsAdvanced){
        Lexer lexer("int i=12*3+6*8+8*3;");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),20);
        ASSERT_EQ(strs->at(0),"push"); 
        ASSERT_EQ(strs->at(1),"12"); 
        ASSERT_EQ(strs->at(2),"push"); 
        ASSERT_EQ(strs->at(3),"3"); 
        ASSERT_EQ(strs->at(4),"times"); 
        ASSERT_EQ(strs->at(5),"push"); 
        ASSERT_EQ(strs->at(6),"6"); 
        ASSERT_EQ(strs->at(7),"push"); 
        ASSERT_EQ(strs->at(8),"8"); 
        ASSERT_EQ(strs->at(9),"times"); 
        ASSERT_EQ(strs->at(10),"push"); 
        ASSERT_EQ(strs->at(11),"8"); 
        ASSERT_EQ(strs->at(12),"push"); 
        ASSERT_EQ(strs->at(13),"3"); 
        ASSERT_EQ(strs->at(14),"times"); 
        ASSERT_EQ(strs->at(15),"add"); 
        ASSERT_EQ(strs->at(16),"add"); 
        ASSERT_EQ(strs->at(17),"store"); 
        ASSERT_EQ(strs->at(18),"i"); 

}


TEST(ByteCode,print){
        Lexer lexer("print(6*2*3);");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),10);
        ASSERT_EQ(strs->at(0),"push"); 
        ASSERT_EQ(strs->at(1),"6"); 
        ASSERT_EQ(strs->at(2),"push"); 
        ASSERT_EQ(strs->at(3),"2"); 
        ASSERT_EQ(strs->at(4),"times"); 
        ASSERT_EQ(strs->at(5),"push"); 
        ASSERT_EQ(strs->at(6),"3"); 
        ASSERT_EQ(strs->at(7),"times"); 
        ASSERT_EQ(strs->at(8),"print"); 
        ASSERT_EQ(strs->at(9),"halt"); 

}


TEST(ByteCode,ifs){
        Lexer lexer("int i=12; int o =13; int right = i < o; if( right ){ print( i ); } else { print(o); }");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),31);
        ASSERT_EQ(strs->at(0),"push"); 
        ASSERT_EQ(strs->at(1),"12"); 
        ASSERT_EQ(strs->at(2),"store"); 
        ASSERT_EQ(strs->at(3),"i"); 
        ASSERT_EQ(strs->at(4),"push"); 
        ASSERT_EQ(strs->at(5),"13"); 
        ASSERT_EQ(strs->at(6),"store"); 
        ASSERT_EQ(strs->at(7),"o"); 
        ASSERT_EQ(strs->at(8),"load"); 
        ASSERT_EQ(strs->at(9),"i"); 
        ASSERT_EQ(strs->at(10),"load"); 
        ASSERT_EQ(strs->at(11),"o"); 
        ASSERT_EQ(strs->at(12),"less"); 
        ASSERT_EQ(strs->at(13),"store"); 
        ASSERT_EQ(strs->at(14),"right"); 
        ASSERT_EQ(strs->at(15),"load"); 
        ASSERT_EQ(strs->at(16),"right"); 
        ASSERT_EQ(strs->at(17),"jif"); 
        ASSERT_EQ(strs->at(18),"startif15"); 
        ASSERT_EQ(strs->at(19),"load"); 
        ASSERT_EQ(strs->at(20),"o"); 
        ASSERT_EQ(strs->at(21),"print"); 
        ASSERT_EQ(strs->at(22),"jmp"); 
        ASSERT_EQ(strs->at(23),"endif15"); 
        ASSERT_EQ(strs->at(24),"startif15:"); 
        ASSERT_EQ(strs->at(25),"load"); 
        ASSERT_EQ(strs->at(26),"i"); 
        ASSERT_EQ(strs->at(27),"print"); 
        ASSERT_EQ(strs->at(28),"endif15:"); 
        ASSERT_EQ(strs->at(29),"halt"); 
}

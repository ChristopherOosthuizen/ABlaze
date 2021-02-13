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
        ASSERT_EQ(strs->size(),7);
        ASSERT_EQ(strs->at(0),"push 8"); 
        ASSERT_EQ(strs->at(1),"push 2"); 
        ASSERT_EQ(strs->at(2),"times"); 
        ASSERT_EQ(strs->at(3),"push 12"); 
        ASSERT_EQ(strs->at(4),"add"); 
        ASSERT_EQ(strs->at(5),"store i"); 
}


TEST(ByteCode,equationsAdvanced){
        Lexer lexer("int i=12*3+6*8+8*3;");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),13);
        ASSERT_EQ(strs->at(0),"push 12"); 
        ASSERT_EQ(strs->at(1),"push 3"); 
        ASSERT_EQ(strs->at(2),"times"); 
        ASSERT_EQ(strs->at(3),"push 6"); 
        ASSERT_EQ(strs->at(4),"push 8"); 
        ASSERT_EQ(strs->at(5),"times"); 
        ASSERT_EQ(strs->at(6),"push 8"); 
        ASSERT_EQ(strs->at(7),"push 3"); 
        ASSERT_EQ(strs->at(8),"times"); 
        ASSERT_EQ(strs->at(9),"add"); 
        ASSERT_EQ(strs->at(10),"add"); 
        ASSERT_EQ(strs->at(11),"store i"); 

}


TEST(ByteCode,print){
        Lexer lexer("print(6*2*3);");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),7);
        ASSERT_EQ(strs->at(0),"push 6"); 
        ASSERT_EQ(strs->at(1),"push 2"); 
        ASSERT_EQ(strs->at(2),"times"); 
        ASSERT_EQ(strs->at(3),"push 3"); 
        ASSERT_EQ(strs->at(4),"times"); 
        ASSERT_EQ(strs->at(5),"print"); 
}



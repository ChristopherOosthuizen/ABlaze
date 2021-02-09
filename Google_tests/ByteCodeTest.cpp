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
        ASSERT_EQ(strs->size(),6);
        ASSERT_EQ(strs->at(0),"create i , IDEN_INT"); 
        ASSERT_EQ(strs->at(1),"asi memp , 8"); 
        ASSERT_EQ(strs->at(2),"TIMES memp , 2"); 
        ASSERT_EQ(strs->at(3),"PLUS memp , 12"); 
        ASSERT_EQ(strs->at(4),"assign [i] , memp"); 
        ASSERT_EQ(strs->at(5),"call"); 
}


TEST(ByteCode,equationsAdvanced){
        Lexer lexer("int i=12*3+6*8+8*3;");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),11);
        ASSERT_EQ(strs->at(0),"create i , IDEN_INT"); 
        ASSERT_EQ(strs->at(1),"asi memp , 12"); 
        ASSERT_EQ(strs->at(2),"TIMES memp , 3"); 
        ASSERT_EQ(strs->at(3),"asi memp , 6"); 
        ASSERT_EQ(strs->at(4),"TIMES memp , 8"); 
        ASSERT_EQ(strs->at(5),"asi memp , 8"); 
        ASSERT_EQ(strs->at(6),"TIMES memp , 3"); 
        ASSERT_EQ(strs->at(7),"PLUS memp , memp"); 
        ASSERT_EQ(strs->at(8),"PLUS memp , memp"); 
        ASSERT_EQ(strs->at(9),"assign [i] , memp"); 
        ASSERT_EQ(strs->at(10),"call"); 

}


TEST(ByteCode,print){
        Lexer lexer("print(6*2*3);");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),5);
        ASSERT_EQ(strs->at(0),"asi memp , 6"); 
        ASSERT_EQ(strs->at(1),"TIMES memp , 2"); 
        ASSERT_EQ(strs->at(2),"TIMES memp , 3"); 
        ASSERT_EQ(strs->at(3),"set func , print"); 
        ASSERT_EQ(strs->at(4),"call"); 
}



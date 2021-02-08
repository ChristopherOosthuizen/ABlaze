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
        ASSERT_EQ(strs->size(),5);
        ASSERT_EQ(strs->at(0),"create i , IDEN_INT"); 
        ASSERT_EQ(strs->at(1),"asi memp , 8"); 
        ASSERT_EQ(strs->at(2),"TIMES memp , 2"); 
        ASSERT_EQ(strs->at(3),"PLUS memp , 12"); 
        ASSERT_EQ(strs->at(4),"assign [i] , memp"); 
}

// Created by Christopher Oosthuizen on 01/27/21
// This class tests ErrorThrowing by ASTGen

#include "gtest/gtest.h"
#include "AST.h"
#include "ASTGen.h"
#include "Lexer.h"
#include "ErrorThrower.h"

TEST(ErrorThrowerAST,basics){
        ErrorThrower::hasError= false;
        ErrorThrower::errors = new vector<string>();
        string content ="int main(){int hello = there; 12; print(\"hello\") int 2 = 12;}"; 
        ErrorThrower::m_content;
        Lexer lexer(content);
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ASSERT_TRUE(ErrorThrower::hasError);
}

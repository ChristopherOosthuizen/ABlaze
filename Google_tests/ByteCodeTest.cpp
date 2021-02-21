#include "gtest/gtest.h"
#include "AST.h"
#include "ASTGen.h"
#include "ErrorThrower.h"
#include "Lexer.h"
#include "ByteGen.h"


TEST(ByteCode,pluss){
        Lexer lexer("i++;");
        ASTGen gen(lexer.readAllTokens());
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),8);
        ASSERT_EQ(strs->at(0),"load"); 
        ASSERT_EQ(strs->at(1),"i"); 
        ASSERT_EQ(strs->at(2),"push"); 
        ASSERT_EQ(strs->at(3),"1"); 
        ASSERT_EQ(strs->at(4),"add"); 
        ASSERT_EQ(strs->at(5),"store"); 
        ASSERT_EQ(strs->at(6),"i"); 
        ASSERT_EQ(strs->at(7),"halt"); 
}

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
        ASSERT_EQ(strs->size(),32);
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
        ASSERT_EQ(strs->at(12),"islt"); 
        ASSERT_EQ(strs->at(13),"store"); 
        ASSERT_EQ(strs->at(14),"right"); 
        ASSERT_EQ(strs->at(15),"startlocal");
        ASSERT_EQ(strs->at(16),"load"); 
        ASSERT_EQ(strs->at(17),"right"); 
        ASSERT_EQ(strs->at(18),"jif"); 
        ASSERT_EQ(strs->at(19),"startif15"); 
        ASSERT_EQ(strs->at(20),"load"); 
        ASSERT_EQ(strs->at(21),"o"); 
        ASSERT_EQ(strs->at(22),"print"); 
        ASSERT_EQ(strs->at(23),"jmp"); 
        ASSERT_EQ(strs->at(24),"endif15"); 
        ASSERT_EQ(strs->at(25),"startif15:"); 
        ASSERT_EQ(strs->at(26),"load"); 
        ASSERT_EQ(strs->at(27),"i"); 
        ASSERT_EQ(strs->at(28),"print"); 
        ASSERT_EQ(strs->at(29),"endif15:"); 
        ASSERT_EQ(strs->at(30),"poplocal");
        ASSERT_EQ(strs->at(31),"halt"); 
}

TEST(ByteCode,whiles){
        Lexer lexer("int i=0; while(i<12){i =i+1; print(i);} ");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),31);
        ASSERT_EQ(strs->at(0),"push"); 
        ASSERT_EQ(strs->at(1),"0"); 
        ASSERT_EQ(strs->at(2),"store"); 
        ASSERT_EQ(strs->at(3),"i"); 
        ASSERT_EQ(strs->at(4),"startWhile4:"); 
        ASSERT_EQ(strs->at(5),"startlocal"); 
        ASSERT_EQ(strs->at(6),"load"); 
        ASSERT_EQ(strs->at(7),"i"); 
        ASSERT_EQ(strs->at(8),"push"); 
        ASSERT_EQ(strs->at(9),"12"); 
        ASSERT_EQ(strs->at(10),"islt"); 
        ASSERT_EQ(strs->at(11),"not"); 
        ASSERT_EQ(strs->at(12),"startlocal"); 
        ASSERT_EQ(strs->at(13),"jif"); 
        ASSERT_EQ(strs->at(14),"endWhile4"); 
        ASSERT_EQ(strs->at(15),"load"); 
        ASSERT_EQ(strs->at(16),"i"); 
        ASSERT_EQ(strs->at(17),"push"); 
        ASSERT_EQ(strs->at(18),"1"); 
        ASSERT_EQ(strs->at(19),"add"); 
        ASSERT_EQ(strs->at(20),"store"); 
        ASSERT_EQ(strs->at(21),"i"); 
        ASSERT_EQ(strs->at(22),"load"); 
        ASSERT_EQ(strs->at(23),"i"); 
        ASSERT_EQ(strs->at(24),"print"); 
        ASSERT_EQ(strs->at(25),"poplocal"); 
        ASSERT_EQ(strs->at(26),"jmp"); 
        ASSERT_EQ(strs->at(27),"startWhile4"); 
        ASSERT_EQ(strs->at(28),"endWhile4:"); 
        ASSERT_EQ(strs->at(29),"poplocal"); 
        ASSERT_EQ(strs->at(30),"halt"); 
}


TEST(ByteCode,fors){
        Lexer lexer("for(int i=0; i<12; i= i+1){print(i);} ");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),31);
        ASSERT_EQ(strs->at(0),"startlocal"); 
        ASSERT_EQ(strs->at(1),"push"); 
        ASSERT_EQ(strs->at(2),"0"); 
        ASSERT_EQ(strs->at(3),"store"); 
        ASSERT_EQ(strs->at(4),"i"); 
        ASSERT_EQ(strs->at(5),"startFor0:"); 
        ASSERT_EQ(strs->at(6),"load"); 
        ASSERT_EQ(strs->at(7),"i"); 
        ASSERT_EQ(strs->at(8),"push"); 
        ASSERT_EQ(strs->at(9),"12"); 
        ASSERT_EQ(strs->at(10),"islt"); 
        ASSERT_EQ(strs->at(11),"not"); 
        ASSERT_EQ(strs->at(12),"startlocal"); 
        ASSERT_EQ(strs->at(13),"jif"); 
        ASSERT_EQ(strs->at(14),"endFor0"); 
       ASSERT_EQ(strs->at(15),"load"); 
        ASSERT_EQ(strs->at(16),"i"); 
        ASSERT_EQ(strs->at(17),"print"); 
        ASSERT_EQ(strs->at(18),"load"); 
        ASSERT_EQ(strs->at(19),"i"); 
        ASSERT_EQ(strs->at(20),"push"); 
        ASSERT_EQ(strs->at(21),"1"); 
        ASSERT_EQ(strs->at(22),"add"); 
        ASSERT_EQ(strs->at(23),"store"); 
        ASSERT_EQ(strs->at(24),"i"); 
        ASSERT_EQ(strs->at(25),"poplocal"); 
        ASSERT_EQ(strs->at(26),"jmp"); 
        ASSERT_EQ(strs->at(27),"startFor0"); 
        ASSERT_EQ(strs->at(28),"endFor0:"); 
        ASSERT_EQ(strs->at(29),"poplocal"); 
        ASSERT_EQ(strs->at(30),"halt"); 
}

TEST(ByteCode,functions){
        Lexer lexer("void  runner(int i){print(i);} int main(){ int o = 12; runner(o)}");
        vector<Token*> tokens = lexer.readAllTokens();
        ASTGen gen(tokens);
        Body* body = gen.generateAST();
        ByteGen byt(body);
        vector<string>* strs = byt.generateByteCode();
        ASSERT_EQ(strs->size(),17);
        ASSERT_EQ(strs->at(0),"runner:"); 
        ASSERT_EQ(strs->at(1),"store"); 
        ASSERT_EQ(strs->at(2),"i"); 
        ASSERT_EQ(strs->at(3),"load"); 
        ASSERT_EQ(strs->at(4),"i"); 
        ASSERT_EQ(strs->at(5),"print"); 
        ASSERT_EQ(strs->at(6),"return"); 
        ASSERT_EQ(strs->at(7),"main:"); 
        ASSERT_EQ(strs->at(8),"push"); 
        ASSERT_EQ(strs->at(9),"12"); 
        ASSERT_EQ(strs->at(10),"store"); 
        ASSERT_EQ(strs->at(11),"o"); 
        ASSERT_EQ(strs->at(12),"load"); 
        ASSERT_EQ(strs->at(13),"o"); 
        ASSERT_EQ(strs->at(14),"call"); 
        ASSERT_EQ(strs->at(15),"runner"); 
        ASSERT_EQ(strs->at(16),"halt"); 
 
}




#include "gtest/gtest.h"
#include "Vm.h"
#include "ByteLexer.h"

TEST(VM,PLUS){
        ByteLexer lexer("push 120 push 6 add halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],126);

}


TEST(VM,MINUS){
         ByteLexer lexer("push 120 push 6 minus halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],-114);

}

TEST(VM,TIMES){
        ByteLexer lexer("push 120 push 6 times halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],720);

}

TEST(VM,DIVIDE){
         ByteLexer lexer("push 120 push 6 divide halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],0);

}


TEST(VM,LOADSTORE){
        ByteLexer lexer("push 12 store i load i push 6 add store i load i halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),14);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],18);

}


TEST(VM,IF){
        ByteLexer lexer("push 12 store i push 13 store o load i load o isgt jif 19 load o jmp 21 load i halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),22);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],13);

}

TEST(VM,CALL){
        ByteLexer lexer("push 25 push 99 call 7 halt store i store o load i load o isgt jif 21 load o return load i return");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),24);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],99);
}

TEST(VM,labels){
        ByteLexer lexer("push 12 store i push 13 store o load i load o isgt jif if load o jmp end if: load i end: halt");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),24);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],13);
}
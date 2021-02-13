
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
        ByteLexer lexer("push 12 store 0 load 0 push 6 add store 1 load 1 halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),14);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],18);

}


TEST(VM,IF){
        ByteLexer lexer("push 12 store 0 push 13 store 1 load 0 load 1 isgt jif 19 load 1 jmp 21 load 0 halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),22);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],13);

}

TEST(VM,CALL){
        ByteLexer lexer("push 25 push 99 call 7 halt store 0 store 1 load 0 load 1 isgt jif 21 load 1 return load 0 return");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),24);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],99);
}

TEST(VM,labels){
        ByteLexer lexer("push 12 store 0 push 13 store 1 load 0 load 1 isgt jif if load 1 jmp end if: load 0 end: halt");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),24);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],13);
}



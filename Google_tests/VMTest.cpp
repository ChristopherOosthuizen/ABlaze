
#include "gtest/gtest.h"
#include "Vm.h"
#include "ByteLexer.h"

TEST(VM,PLUS){
        ByteLexer lexer("push 120 push 6 add halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int,126);

}

TEST(VM,strings){
        ByteLexer lexer("push \"hello\" push \" there\" add halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_type,ByteType::STRING);

       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_string,"hello there");

}


TEST(VM,Double){
        ByteLexer lexer("push 120.4 push 6.5 add halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_type,ByteType::DOUBLE);

       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_double,126.9);

}


TEST(VM,isle){
        ByteLexer lexer("push 12 push 14 isle halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,1);

}

TEST(VM,isge){
        ByteLexer lexer("push 12 push 14 isge halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,0);

}

TEST(VM,isgt){
        ByteLexer lexer("push 12 push 14 isgt halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,0);

}



TEST(VM,nots){
        ByteLexer lexer("push 12 push 14 isgt not halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,1);

}



TEST(VM,islt){
        ByteLexer lexer("push 12 push 14 islt halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,1);

}



TEST(VM,MINUS){
         ByteLexer lexer("push 120 push 6 minus halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,114);

}

TEST(VM,TIMES){
        ByteLexer lexer("push 120 push 6 times halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,720);

}

TEST(VM,DIVIDE){
         ByteLexer lexer("push 120 push 6 divide halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,20);

}


TEST(VM,LOADSTORE){
        ByteLexer lexer("push 12 store i load i push 6 add store i load i halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),14);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,18);

}


TEST(VM,IF){
        ByteLexer lexer("push 12 store i push 13 store o load i load o isgt jif 19 load o jmp 21 load i halt");
        vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,13);

}

TEST(VM,CALL){
        ByteLexer lexer("push 25 push 99 call 7 halt store i store o load i load o isgt jif 21 load o return load i return");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),24);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,99);
}

TEST(VM,labels){
        ByteLexer lexer("push 12 store i push 13 store o load i load o isgt jif if load o jmp end if: load i end: halt");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(tokens.size(),24);
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,13);
}

TEST(VM,locals){
        ByteLexer lexer("call  main  main:  push  12  store  i  startlocal  push  1  jif  startif7  jmp  endif7  startif7:  push  14  store  i  load  i  print  endif7:  poplocal  startlocal  push  1  jif  startif24  jmp  endif24  startif24:  push  15  store  i  load  i  print  endif24:  poplocal  load  i  print  halt  ");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),0);
}

TEST(VM,forLoop){
        ByteLexer lexer("call main main: startlocal push 0 store i startFor3: load i push 20 islt not startlocal jif endFor3 load i print load i push 1 add asi i poplocal jmp startFor3 endFor3: poplocal halt");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),0);

}


TEST(VM,recursion){
        ByteLexer lexer("call main right: store i startlocal load i push 2 equal_equal jif startif5 jmp endif5 startif5: push 12 return endif5: poplocal push 4 push 2 call right add return return main: push 3 call right print halt ");
          vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),0);

}

TEST(VM,array){
       ByteLexer lexer("new list store i load i push 0 append load i push 3 append load i push 0 delete load i push 0 at halt");
       vector<ByteToken*> tokens = lexer.readAllTokens();
       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack.size(),1);
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1].m_val.m_int ,3);

}





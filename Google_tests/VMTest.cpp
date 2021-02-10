
#include "gtest/gtest.h"
#include "Vm.h"

TEST(VM,PLUS){
        vector<ByteToken*> tokens = {new ByteToken(ByteType::PUSH,0),new ByteToken(ByteType::INT,120), new ByteToken(ByteType::PUSH,0), new ByteToken(ByteType::INT,6), new ByteToken(ByteType::ADD,0),new ByteToken(ByteType::HALT,0)};

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],126);

}


TEST(VM,MINUS){
        vector<ByteToken*> tokens = {new ByteToken(ByteType::PUSH,0),new ByteToken(ByteType::INT,120), new ByteToken(ByteType::PUSH,0), new ByteToken(ByteType::INT,6), new ByteToken(ByteType::MINUS,0),new ByteToken(ByteType::HALT,0)};

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],-114);

}

TEST(VM,TIMES){
        vector<ByteToken*> tokens = {new ByteToken(ByteType::PUSH,0),new ByteToken(ByteType::INT,120), new ByteToken(ByteType::PUSH,0), new ByteToken(ByteType::INT,6), new ByteToken(ByteType::TIMES,0),new ByteToken(ByteType::HALT,0)};

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],720);

}

TEST(VM,DIVIDE){
        vector<ByteToken*> tokens = {new ByteToken(ByteType::PUSH,0),new ByteToken(ByteType::INT,120), new ByteToken(ByteType::PUSH,0), new ByteToken(ByteType::INT,6), new ByteToken(ByteType::DIVIDE,0),new ByteToken(ByteType::HALT,0)};

       Vm vm(tokens); 
       vm.execute();
       ASSERT_EQ(vm.m_stack[vm.m_stack.size()-1],0);

}


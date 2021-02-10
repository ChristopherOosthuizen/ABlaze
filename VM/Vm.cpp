#include "Vm.h"

Vm::Vm(vector<ByteToken*>& tokens){
       m_pos =0;
        m_tokens = tokens;
        m_halted = false;
}

void Vm::execute(){
        while(!m_halted){
                step();
        }
}

void Vm::step(){
        ByteToken* token = m_tokens[m_pos++];
        ByteType type = token->m_type;
       switch(type){
                case ByteType::PUSH:pushToStack();break;
                case ByteType::ADD:
                case ByteType::MINUS:
                case ByteType::TIMES:
                case ByteType::DIVIDE: binOP(token->m_type);break;
                case ByteType::HALT:
                                       m_halted = true; break;
       }
}

void Vm::pushToStack(){
        m_stack.push_back(m_tokens[m_pos++]->m_value);
}

void Vm::binOP(ByteType type){
        int result =0;
        int one = m_stack[m_stack.size()-1];
                m_stack.pop_back();
        int two = m_stack[m_stack.size()-1];
                m_stack.pop_back();
        switch(type){
                case ByteType::ADD:
                        result = one + two;break;
                case ByteType::MINUS:
                        result = one - two;break;
                case ByteType::TIMES:
                        result = one * two;break;
                case ByteType::DIVIDE:
                        result = one / two;break;
        }
        m_stack.push_back(result);

}


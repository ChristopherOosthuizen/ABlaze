#include "Vm.h"

Vm::Vm(vector<ByteToken*>& tokens){
       m_pos =0;
        m_tokens = tokens;
        m_halted = false;
        m_jumpBack =0;
}

void Vm::execute(){
        for(int i=0; i< m_tokens.size(); i++){
                if(m_tokens[i]->m_type == ByteType::LABEL){
                        m_labels[m_tokens[i]->m_symbol] = i;
                }
        }
        while(!m_halted){
                step();
        }
}

void Vm::step(){
        ByteToken* token = m_tokens[m_pos++];
        ByteType type = token->m_type;
       switch(type){
                case ByteType::PUSH:pushToStack();break;
                case ByteType::JIF: jumpIf(); break;
                case ByteType::JMP: jump();break;
                case ByteType::RETURN: Return();break;
                case ByteType::CALL: call(); break;
                case ByteType::ADD:
                case ByteType::MINUS:
                case ByteType::TIMES:
                case ByteType::XOR:
                case ByteType::NOT:
                case ByteType::AND:
                case ByteType::OR:
                case ByteType::ISGT:
                case ByteType::ISLT:
                case ByteType::ISLE:
                case ByteType::ISGE:
                case ByteType::DIVIDE: binOP(token->m_type);break;
                case ByteType::LOAD: load(); break;
                case ByteType::STORE: store(); break;
                case ByteType::HALT:
                                       m_halted = true; break;
       }
}

void Vm::jumpIf(){
        if(m_stack[m_stack.size()-1] ==1)
                jump();
        m_stack.pop_back();
}

void Vm::jump(){
        if(m_tokens[m_pos]->m_type == ByteType::INT)
                m_pos = m_tokens[m_pos]->m_value;
        else
                m_pos = m_labels[m_tokens[m_pos]->m_symbol];
}

void Vm::pushToStack(){
        m_stack.push_back(m_tokens[m_pos++]->m_value);
}

void Vm::load(){
        m_stack.push_back(m_vars[m_tokens[m_pos++]->m_value]);
}

void Vm::store(){
        m_vars[m_tokens[m_pos++]->m_value] = m_stack[m_stack.size()-1];
        m_stack.pop_back();
}

void Vm::Return(){
        m_pos = m_jumpBack;
        m_jumpBack = 0;
}

void Vm::call(){
        m_jumpBack = m_pos+1;
        jump();
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
                case ByteType::XOR:
                        result = one ^ two; break;
                case ByteType::AND:
                        result = one & two; break;
                case ByteType::OR:
                        result = one | two; break;
                case ByteType::ISGT:
                        result = one < two; break;
                case ByteType::ISLT:
                        result = one > two; break;
                case ByteType::ISLE:
                        result = one <= two; break;
                case ByteType::ISGE:
                        result = one >= two; break;

        }
        m_stack.push_back(result);

}


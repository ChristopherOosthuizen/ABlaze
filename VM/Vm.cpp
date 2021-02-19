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
                case ByteType::PRINT: print(); break;
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
        if(m_stack[m_stack.size()-1].m_val.m_int ==1)
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
        DataVal val(m_tokens[m_pos]->m_type,Val(m_tokens[m_pos]->m_value,stod(m_tokens[m_pos]->m_symbol) ,m_tokens[m_pos]->m_value));
        m_pos++;
        m_stack.push_back(val);
}

void Vm::load(){
        m_stack.push_back(m_vars[m_tokens[m_pos++]->m_symbol]);
}

void Vm::store(){
        m_vars[m_tokens[m_pos++]->m_symbol] = m_stack[m_stack.size()-1];
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

void Vm::print(){
        cout<< m_stack[m_stack.size()-1].m_val.m_int<<endl;
        m_stack.pop_back();
}

void Vm::binOP(ByteType type){
        int result =0;

        DataVal one = m_stack[m_stack.size()-1];
        m_stack.pop_back();
        if(type==ByteType::NOT){
                int result =1;
                if(one.m_val.m_int ==1){
                        result = 0;

                }
                DataVal val(ByteType::INT,Val(result,-1,-1));
                m_stack.push_back(val);

                return;
        }
        DataVal two = m_stack[m_stack.size()-1];
        m_stack.pop_back();
        if(two.m_type == ByteType::DOUBLE||one.m_type == ByteType::DOUBLE ){
                binOPDouble(type, one.m_val.m_double , two.m_val.m_double);
                return;
        }

        switch(type){
                case ByteType::ADD:
                        result = two.m_val.m_int + one.m_val.m_int;break;
                case ByteType::MINUS:
                        result = two.m_val.m_int - one.m_val.m_int;break;
                case ByteType::TIMES:
                        result = two.m_val.m_int * one.m_val.m_int;break;
                case ByteType::DIVIDE:
                        result = two.m_val.m_int / one.m_val.m_int;break;
                case ByteType::XOR:
                        result = two.m_val.m_int ^ one.m_val.m_int; break;
                case ByteType::AND:
                        result = two.m_val.m_int & one.m_val.m_int; break;
                case ByteType::OR:
                        result = two.m_val.m_int | one.m_val.m_int; break;
                case ByteType::ISGT:
                        result = two.m_val.m_int > one.m_val.m_int; break;
                case ByteType::ISLT:
                        result = two.m_val.m_int < one.m_val.m_int; break;
                case ByteType::ISLE:
                        result = two.m_val.m_int <= one.m_val.m_int; break;
                case ByteType::ISGE:
                        result = two.m_val.m_int >= one.m_val.m_int; break;

        }
        DataVal val(ByteType::INT,Val(result,result,result));
        m_stack.push_back(val);

}

void Vm::binOPDouble(ByteType type, double one , double two){
        double result =0;
        switch(type){
                case ByteType::ADD:
                        result = two + one;break;
                case ByteType::MINUS:
                        result = two - one;break;
                case ByteType::TIMES:
                        result = two * one;break;
                case ByteType::DIVIDE:
                        result = two / one;break;
                case ByteType::ISGT:
                        result = two > one; break;
                case ByteType::ISLT:
                        result = two < one; break;
                case ByteType::ISLE:
                        result = two <= one; break;
                case ByteType::ISGE:
                        result = two >= one; break;

        }
        ByteType typer = ByteType::DOUBLE;
        switch(type){
                case ByteType::ISGT: 
                case ByteType::ISLT: 
                case ByteType::ISLE: 
                case ByteType::ISGE: typer = ByteType::INT;


        }
        DataVal val(typer,Val(result,result,result));
        m_stack.push_back(val);
 
}

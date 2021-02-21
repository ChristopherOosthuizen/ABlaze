#include "Vm.h"

Vm::Vm(vector<ByteToken*>& tokens){
       m_pos =0;
        m_tokens = tokens;
        m_halted = false;
        m_localCount= 0;
        m_locals.push_back(new vector<Local*>());
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
                case ByteType::HALT:m_halted = true; break;
                case ByteType::CREATELOCAL:createLocal();break;
                case ByteType::POPLOCAL:popLocal();break;
 
       }
}

void Vm::createLocal(){
       m_localCount++; 
}

void Vm::popLocal(){
        vector<Local*>* locals = m_locals.at(m_locals.size()-1);
        while(locals->size() > 0 && locals->at(m_locals.size()-1)->m_depth == m_localCount){
              Local* local =locals->at(m_locals.size()-1);   
              m_locals.pop_back();
              delete local;
        }
        if(m_localCount!=0)
                m_localCount--;
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
        double value =0;
        if(m_tokens[m_pos]->m_type != ByteType::STRING)
                value = stod(m_tokens[m_pos]->m_symbol); 
        DataVal val(m_tokens[m_pos]->m_type,Val(m_tokens[m_pos]->m_value, value ,m_tokens[m_pos]->m_value,m_tokens[m_pos]->m_symbol));
        m_pos++;
        m_stack.push_back(val);
}

void Vm::load(){
        string name=m_tokens[m_pos++]->m_symbol; 
        vector<Local*>* locals = m_locals[m_locals.size()-1];
        for(int i=locals->size()-1;i >=0;i--){
                if(locals->at(i)->m_name ==name){
                        m_stack.push_back(locals->at(i)->m_val);
                        return;
                }
        }
}

void Vm::store(){
        Local* local = NULL;
        string name =m_tokens[m_pos++]->m_symbol; 
        vector<Local*>* locals = m_locals[m_locals.size()-1];
        for(int i = locals->size()-1; i>=0&&locals->at(i)->m_depth == m_localCount ; i--){
                if(locals->at(i)->m_name == name){
                        local = locals->at(i);
                        local->m_val =m_stack[m_stack.size()-1]; 
                        break;
                }
        }
        if(local ==NULL)
                locals->push_back(new Local(m_localCount,name, m_stack[m_stack.size()-1]));
        m_stack.pop_back();
}

void Vm::Return(){
        m_pos = m_jumpBacks[m_jumpBacks.size()-1];
        vector<Local*>* locals = m_locals[m_locals.size()-1];
        m_locals.pop_back();
        delete locals;
        m_jumpBacks.pop_back();
}

void Vm::call(){
        m_jumpBacks.push_back(m_pos+1);
        m_locals.push_back(new vector<Local*>());
        jump();
}

void Vm::print(){
        cout<< m_stack[m_stack.size()-1].m_val.m_string<<endl;

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
                DataVal val(ByteType::INT,Val(result,-1,-1,to_string(result)));
                m_stack.push_back(val);

                return;
        }
        DataVal two = m_stack[m_stack.size()-1];
        m_stack.pop_back();
        if(two.m_type == ByteType::STRING||one.m_type == ByteType::STRING){
                binOPSTRING(type, one.m_val.m_string, two.m_val.m_string);
                return;

        }
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
        DataVal val(ByteType::INT,Val(result,result,result,to_string(result)));
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
        DataVal val(typer,Val(result,result,result,to_string(result)));
        m_stack.push_back(val);
 
}

void Vm::binOPSTRING(ByteType type, string one, string two){
        string result = "";
        switch(type){
                case ByteType::ADD:
                        result = two+one;break;
        }
        DataVal val(ByteType::STRING,Val(0,0,0,result));
        m_stack.push_back(val);

}

#include "Vm.h"
#include <math.h>
#include<stdio.h>
#include <iostream>     // std::cout
#include <fstream>  
#include <cstdio>

Vm::Vm(vector<ByteToken*>& tokens){
       m_pos =0;
        m_tokens = tokens;
        m_halted = false;
        m_localCounts.push_back(0);
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

void Vm::garbageCollect(int pos){
        for(vector<Local*>* local:m_locals){
                for(int i=0; i< local->size(); i++){
                        if( local->at(i)->m_val.m_type == ByteType::OBJ && local->at(i)->m_val.m_val.m_int ==pos){
                                return;
                        }
                }
        }
        for(int i= 0; i< m_stack.size() ;i++){
                if( m_stack[i].m_type == ByteType::OBJ &&  m_stack[i].m_val.m_int ==pos){
                                return;
                }
        }
        DataObj* obj = m_objs[pos];
        m_objs[pos] = nullptr;
        delete obj;
}

void Vm::collectAllGarbage(){
        for(int i=0; i< m_objs.size(); i++){
                if(m_objs[i] != nullptr)
                        garbageCollect(i);
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
                case ByteType::NEW: newObj();break;
                case ByteType::DELETE: del(); break;
                case ByteType::AT: at(); break;
                case ByteType::APPEND: append(); break;
                case ByteType::ADD:
                case ByteType::MINUS:
                case ByteType::TIMES:
                case ByteType::XOR:
                case ByteType::NOT:
                case ByteType::AND:
                case ByteType::OR:
                case ByteType::ISGT:
                case ByteType::EQUAL:
                case ByteType::ISLT:
                case ByteType::ISLE:
                case ByteType::POW:
                case ByteType::SQRT:
                case ByteType::ISGE:
                case ByteType::DIVIDE: binOP(token->m_type);break;
                case ByteType::STRUCTCONST: structDec();break;
                case ByteType::LOAD: load(); break;
                case ByteType::STORE: store(); break;
                case ByteType::ASI: asi();break;
                case ByteType::HALT:m_halted = true; break;
                case ByteType::CREATELOCAL:createLocal();break;
                case ByteType::POPLOCAL:popLocal();break;
                case ByteType::SET: set(); break;
                case ByteType::SELECT: select(); break;
                case ByteType::CAST: cast(); break;
                case ByteType::EXISTS: exists(); break;
                case ByteType::READFILE: readFile(); break;
                case ByteType::WRITEFILE: writeFile(); break;
                case ByteType::DELETEFILE: deleteFile(); break;
                case ByteType::CREATEFILE: createFile(); break;
                case ByteType::INPUT: input(); break;

 
       }
       collectAllGarbage();
}

void Vm::exists(){
        ifstream ifile;
        string str = m_stack[m_stack.size()-1].m_val.m_string;
        ifile.open(str);
        m_stack.pop_back();
        int res = 0;
        if(ifile)
                res =1;
        DataVal val(ByteType::INT,Val(res,res,res,to_string(res)));
        m_stack.push_back(val);
}

void Vm::readFile(){
        string line = "";
        string path= m_stack[m_stack.size()-1].m_val.m_string;
        m_stack.pop_back();

        vector<string>* strs = new vector<string>();
        ifstream reader(path);
        while ( getline ( reader , line ) ) {
                strs->push_back(line);
        }
        int pos = m_objs.size();
        DataVal val(ByteType::OBJ, Val(pos,pos,pos,""));     
        m_objs.push_back(new DataObj(ByteType::LIST,strs));
        m_stack.push_back(val);
 

}

void Vm::writeFile(){
        string content =m_stack[m_stack.size()-1].m_val.m_string;
        m_stack.pop_back(); 
        string str = m_stack[m_stack.size()-1].m_val.m_string;
        m_stack.pop_back();

        ofstream file(str, std::ios_base::app);
                file << content;
        file.close();

}

void Vm::deleteFile(){
        string str = m_stack[m_stack.size()-1].m_val.m_string;
        m_stack.pop_back();
        remove(str.c_str());
}

void Vm::createFile(){
        string str = m_stack[m_stack.size()-1].m_val.m_string;
        m_stack.pop_back();

        ofstream file(str);
        file.close();
}

void Vm::cast(){
        DataVal val = m_stack[m_stack.size()-1];
        m_stack.pop_back();
        string name = m_tokens[m_pos++]->m_symbol;
        if(name == "int"){
                if(val.m_type == ByteType::STRING){
                        int value = stoi(val.m_val.m_string);
                        val.m_val.m_double= value;
                        val.m_val.m_int = value;
                        val.m_val.m_char = value;
                }
                val.m_type = ByteType::INT;                
        }else if(name == "double"){
              if(val.m_type == ByteType::STRING){
                        double value = stod(val.m_val.m_string);
                        val.m_val.m_double= value;
                        val.m_val.m_int = (int)value;
                        val.m_val.m_char = (int)value;
                }
                val.m_type = ByteType::DOUBLE;                
        }else if(name == "char"){
                if(val.m_type == ByteType::STRING){
                        int value = val.m_val.m_string[0];
                        val.m_val.m_double= value;
                        val.m_val.m_int = value;
                        val.m_val.m_char = value;
                }
                val.m_type = ByteType::CHAR;                
        }else if(name == "string"){
                val.m_type=ByteType::STRING;
        }
        m_stack.push_back(val);

}

void Vm::input(){
        string res;
        std::getline(cin,res);
        m_stack.push_back(DataVal(ByteType::STRING,Val(0,0,0,res)));
}

void Vm::set(){
        DataVal two = m_stack[m_stack.size()-1];
        m_stack.pop_back();
        DataVal one = m_stack[m_stack.size()-1];
        m_stack.pop_back();
        StructObj* obj = (StructObj*)m_objs[two.m_val.m_int]->m_pointer;
        obj->m_vals[m_tokens[m_pos++]->m_symbol] = one;
}


void Vm::select(){
        DataVal two = m_stack[m_stack.size()-1];
        m_stack.pop_back();
        StructObj* obj = (StructObj*)m_objs[two.m_val.m_int]->m_pointer;
        m_stack.push_back(obj->m_vals[m_tokens[m_pos++]->m_symbol]);
}

void Vm::structDec(){
        vector<string>* strs = new vector<string>();
        vector<Local*>* locals = m_locals.at(m_locals.size()-1);
        while(locals->size() > 0 && locals->at(locals->size()-1)->m_depth == m_localCounts[m_localCounts.size()-1]){
              Local* local =locals->at(m_locals.size()-1);   
              locals->pop_back();
              strs->push_back(local->m_val.m_val.m_string);
              delete local;
        }
        if(m_localCounts[m_localCounts.size()-1]!=0)
                m_localCounts[m_localCounts.size()-1]--;
        ByteToken* token= m_tokens[m_pos++];
        (m_structs)[token->m_symbol] = strs;
}

void Vm::append(){
        DataVal two = m_stack[m_stack.size()-1];
        m_stack.pop_back();
        DataVal one = m_stack[m_stack.size()-1];
        m_stack.pop_back();
          
        DataObj* obj = m_objs[one.m_val.m_int];
        vector<DataVal>* vals = (vector<DataVal>*)obj->m_pointer;
        vals->push_back(two);
}

void Vm::at(){
        DataVal two= m_stack[m_stack.size()-1];
        m_stack.pop_back();
        DataVal one= m_stack[m_stack.size()-1];
        m_stack.pop_back();
          
        DataObj* obj = m_objs[one.m_val.m_int];
        vector<DataVal>* vals = (vector<DataVal>*)obj->m_pointer;
        m_stack.push_back(vals->at(two.m_val.m_int));

}

void Vm::del(){
        DataVal two= m_stack[m_stack.size()-1];
        m_stack.pop_back();
        DataVal one= m_stack[m_stack.size()-1];
        m_stack.pop_back();
          
        DataObj* obj = m_objs[one.m_val.m_int];
        vector<DataVal>* vals = (vector<DataVal>*)obj->m_pointer;
        vals->erase(vals->begin()+two.m_val.m_int);


}

void Vm::newObj(){
        ByteToken* token= m_tokens[m_pos++]; 
        int pos = -1;
        for(int i =0; i< m_objs.size(); i++){
                if(m_objs[i] == nullptr){
                        pos = i;
                        break;
                }
        }
        if(pos == -1){
                pos = m_objs.size();
                m_objs.push_back(nullptr);
        }
        DataVal val(ByteType::OBJ, Val(pos,0,0,""));     
        if(token->m_symbol == "list"){          
                m_objs[pos] = new DataObj(ByteType::LIST,new vector<DataVal>());
        }else{            
                m_objs[pos] = new DataObj(ByteType::STRUCT,new StructObj(m_structs[token->m_symbol]));
        }
        m_stack.push_back(val);
}


void Vm::asi(){
        Local* local = NULL;
        string name =m_tokens[m_pos++]->m_symbol; 
        vector<Local*>* locals = m_locals[m_locals.size()-1];
        for(int i = locals->size()-1; i>=0; i--){
                if(locals->at(i)->m_name == name){
                        local = locals->at(i);
                        local->m_val =m_stack[m_stack.size()-1]; 
                        break;
                }
        }
        m_stack.pop_back();
        
}

void Vm::createLocal(){
       m_localCounts[m_localCounts.size()-1]++;
}

void Vm::popLocal(){
        vector<Local*>* locals = m_locals.at(m_locals.size()-1);
        while(locals->size() > 0 && locals->at(locals->size()-1)->m_depth == m_localCounts[m_localCounts.size()-1]){
              Local* local =locals->at(m_locals.size()-1);   
              locals->pop_back();
              delete local;
        }
        if(m_localCounts[m_localCounts.size()-1]!=0)
                m_localCounts[m_localCounts.size()-1]--;
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
        for(int i = locals->size()-1; i>=0&&locals->at(i)->m_depth == m_localCounts[m_localCounts.size()-1] ; i--){
                if(locals->at(i)->m_name == name){
                        local = locals->at(i);
                        local->m_val =m_stack[m_stack.size()-1]; 
                        break;
                }
        }
        if(local ==NULL){
                DataVal val(ByteType::INT,Val(0,0,0,""));
                if(m_stack.size() >0)
                        val = m_stack[m_stack.size()-1];
                locals->push_back(new Local(m_localCounts[m_localCounts.size()-1],name, val));
        }
        if(m_stack.size() >0)
                m_stack.pop_back();
}

void Vm::Return(){
        m_pos = m_jumpBacks[m_jumpBacks.size()-1];
        vector<Local*>* locals = m_locals[m_locals.size()-1];
        m_locals.pop_back();
        delete locals;
        m_jumpBacks.pop_back();
        m_localCounts.pop_back();
}

void Vm::call(){
        m_jumpBacks.push_back(m_pos+1);
        vector<Local*>* locals = new vector<Local*>();
        for(int i=0; i< m_locals[0]->size(); i++){
                locals->push_back(m_locals[0]->at(i));
        }
        m_locals.push_back(locals);
        m_localCounts.push_back(1);
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
        if(two.m_type == ByteType::DOUBLE||one.m_type == ByteType::DOUBLE || type == ByteType::POW || type == ByteType::SQRT){
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

                case ByteType::EQUAL:
                        result = two.m_val.m_int == one.m_val.m_int; break;


        }
        ByteType tip= ByteType::INT;
        DataVal val(tip,Val(result,result,result,to_string(result)));
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
                case ByteType::EQUAL:
                        result = two ==one; break;
                case ByteType::POW:
                        result = pow(two,one);break; 
                case ByteType::SQRT:
                        result = pow(two,1/one);break; 



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

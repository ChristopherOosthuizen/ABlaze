#include "Vm.h"
#include <math.h>
#include<stdio.h>
#include <iostream>     
#include <fstream>  
#include <cstdio>

DataVal Vm::popStack(){
        DataVal val =m_stack[m_stack.size()-1];
        m_stack.pop_back(); 
        return val;
}

string Vm::popStackString(){
        return popStack().m_val.m_string;
}
ByteToken* Vm::nextToken(){
        return m_tokens[m_pos++];
}

Vm::Vm(vector<ByteToken*>& tokens){
       m_pos =0;
        m_tokens = tokens;
        m_halted = false;
        m_hasError = false;
        m_localCounts.push_back(0);
        m_locals.push_back(new vector<Local*>());
}

void Vm::execute(){
        for(int i=0; i< m_tokens.size(); i++){
                if(m_tokens[i]->m_type == ByteType::LABEL){
                        m_labels[m_tokens[i]->m_symbol] = i;
                }
        }
        while(!m_halted && !m_hasError){
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
        ByteToken* token = nextToken(); 
        ByteType type = token->m_type;
       switch(type){
                case ByteType::PUSH:pushToStack();break;
                case ByteType::JIF: jumpIf(); break;
                case ByteType::JMP: jump();break;
                case ByteType::RETURN: Return();break;
                case ByteType::CALL: call(); break;
                case ByteType::PRINTLN: println(); break;
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
                case ByteType::STRUCTCONSTEX: structDecEx();break;
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
                case ByteType::LEN: len();break;
                case ByteType::LOADCLASS: loadclass(); break;
                case ByteType::CLASSCALL: classcall(); break;

 
       }
       collectAllGarbage();
}

void Vm::classcall(){
      DataVal val = m_stack[m_stack.size()-1]; 
      StructObj* obj = (StructObj*)m_objs[val.m_val.m_int]->m_pointer;
      string objectName = (obj->m_name);
      string funcName = nextToken()->m_symbol;
      string name = objectName+"."+funcName;
        m_jumpBacks.push_back(m_pos+1);
        vector<Local*>* locals = new vector<Local*>();
        for(int i=0; i< m_locals[0]->size(); i++){
                locals->push_back(m_locals[0]->at(i));
        }
        m_locals.push_back(locals);
        m_localCounts.push_back(1);

        m_pos = m_labels[name];
      
}

void Vm::loadclass(){
        vector<Local*>* locals = m_locals[m_locals.size()-1];
        DataVal val = m_stack[m_stack.size()-1];
        StructObj* obj = (StructObj*)m_objs[val.m_val.m_int]->m_pointer;
        for(Local* local:*obj->m_vals){
                locals->push_back(local);
        }
}

void Vm::runTimeError(string error){
        perror(error.c_str());
        m_hasError = true;
}

void Vm::len(){
       DataVal val = popStack();
       int res; 
       if(val.m_type == ByteType::OBJ){
               DataObj* obj =m_objs[val.m_val.m_int]; 
                if(obj->m_type == ByteType::LIST){
                        vector<DataVal>* list = (vector<DataVal>*)obj->m_pointer;
                        res = list->size();
                } else
                        res = -1;
       }else{
               res = val.m_val.m_string.size();
       }
       DataVal vals(ByteType::INT,Val(res,res,res,to_string(res)));
       m_stack.push_back(vals);

}

void Vm::exists(){
        ifstream ifile;
        string str = popStackString();
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
        string path= popStackString(); 

        vector<string>* strs = new vector<string>();
        ifstream reader(path);
        if(!reader.good()){
                runTimeError("File "+path+" does not exist");
                return;
        }
        while ( getline ( reader , line ) ) {
                strs->push_back(line);
        }
        int pos = m_objs.size();
        DataVal val(ByteType::OBJ, Val(pos,pos,pos,""));     
        m_objs.push_back(new DataObj(ByteType::LIST,strs));
        m_stack.push_back(val);
 

}

void Vm::writeFile(){
        string content = popStackString();
        string str = popStackString();

        ofstream file(str, std::ios_base::app);
                file << content;
        file.close();

}

void Vm::deleteFile(){
        string str = popStackString(); 
        remove(str.c_str());
}

void Vm::createFile(){
        string str = popStackString(); 

        ofstream file(str);
        file.close();
}

void Vm::cast(){
        DataVal val = popStack(); 
        string name = nextToken()->m_symbol; 
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
        DataVal two = popStack(); 
        StructObj* obj = (StructObj*)m_objs[two.m_val.m_int]->m_pointer;
        asi(obj->m_vals);
}


void Vm::select(){
        DataVal two = popStack(); 
        StructObj* obj = (StructObj*)m_objs[two.m_val.m_int]->m_pointer;
        load(obj->m_vals,nextToken()->m_symbol); 
}

void Vm::pushLocals(vector<Local*>* ins,vector<Local*>* outs){
        int depth = outs->at(outs->size()-1)->m_depth;
        for(int i=outs->size()-1; i>=0 && outs->at(i)->m_depth == depth;i--){
                Local* local = outs->at(i);
                ins->push_back(new Local(0,local->m_name,local->m_val));
        }
}

void Vm::structDecEx(){
        string name = nextToken()->m_symbol;
        string extends = nextToken()->m_symbol;
        vector<Local*>* locals = new vector<Local*>();
        pushLocals(locals,m_structs[extends]);
        pushLocals(locals,m_locals[m_locals.size()-1]);
        m_structs[name] = locals; 
        popLocal();
}


void Vm::structDec(){
        vector<Local*>* locals = new vector<Local*>();
        pushLocals(locals,m_locals[m_locals.size()-1]);
        m_structs[nextToken()->m_symbol] = locals; 
        popLocal();
}

void Vm::append(){
        DataVal two = popStack(); 
        DataVal one = popStack(); 
          
        DataObj* obj = m_objs[one.m_val.m_int];
        vector<DataVal>* vals = (vector<DataVal>*)obj->m_pointer;
        vals->push_back(two);
}

void Vm::at(){
        DataVal two= popStack(); 
        DataVal one= popStack();          
       int index =two.m_val.m_int; 
        
        if(one.m_type == ByteType::STRING){
                char c = one.m_val.m_string[index];
                string symbol;
                symbol =c;
                DataVal val(ByteType::CHAR,Val(c,c,c,symbol));
                m_stack.push_back(val);
                return;
        }
        DataObj* obj = m_objs[one.m_val.m_int];
        vector<DataVal>* vals = (vector<DataVal>*)obj->m_pointer;
        if(index >=vals->size()){
                runTimeError("Index out of bounds for value "+to_string(index));
                return;
        } 
        m_stack.push_back(vals->at(index));

}

void Vm::del(){
        DataVal two= popStack(); 
        DataVal one= popStack(); 
          
        DataObj* obj = m_objs[one.m_val.m_int];
        vector<DataVal>* vals = (vector<DataVal>*)obj->m_pointer;
        vals->erase(vals->begin()+two.m_val.m_int);


}

void Vm::newObj(){
        ByteToken* token= nextToken(); 
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
                m_objs[pos] = new DataObj(ByteType::STRUCT,new StructObj((token->m_symbol), m_structs[token->m_symbol]));
                if(m_labels.count(token->m_symbol) ==1){
                        m_pos--;
                        call();
                }
        }
        m_stack.push_back(val);
}


void Vm::asi(){
        asi(m_locals[m_locals.size()-1]);        
}

void Vm::asi(vector<Local*>* locals){
        Local* local = NULL;
        string name =nextToken()->m_symbol; 
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
        ByteToken* token = m_tokens[m_pos];
        DataVal val(token->m_type,Val(token->m_value, value ,token->m_value,token->m_symbol));
        m_pos++;
        m_stack.push_back(val);
}

void Vm::load(){
        string name=nextToken()->m_symbol; 
        vector<Local*>* locals = m_locals[m_locals.size()-1];
        load(locals,name); 
}

void Vm::load(vector<Local*>* locals, const string& name){
        for(int i=locals->size()-1;i >=0;i--){
                if(locals->at(i)->m_name ==name){
                        m_stack.push_back(locals->at(i)->m_val);
                        return;
                }
        }

}

void Vm::store(){
        Local* local = NULL;
        string name = nextToken()->m_symbol;
        vector<Local*>* locals = m_locals[m_locals.size()-1];
        for(int i = locals->size()-1; i>=0&&locals->at(i)->m_depth == (m_localCounts.back()-1) ; i--){
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
        m_pos = m_jumpBacks.back()-1;
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
        string val = popStackString();
        cout<< val;

}

void Vm::println(){
        string val = popStackString();
        cout << val<<endl;
}

void Vm::binOP(ByteType type){
        int result =0;

        DataVal one = popStack(); 
        if(type==ByteType::NOT){
                int result =1;
                if(one.m_val.m_int ==1){
                        result = 0;

                }
                DataVal val(ByteType::INT,Val(result,-1,-1,to_string(result)));
                m_stack.push_back(val);

                return;
        }
        DataVal two = popStack(); 
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
                case ByteType::EQUAL:
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

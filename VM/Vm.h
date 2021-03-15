

#ifndef ABLAZE_VM_H
#define ABLAZE_VM_H
#include <vector>
#include <map>
#include <iostream>
#include "ByteToken.h"
using namespace std;

                


class Val{
public:
        int m_int;
        double m_double;
        char m_char;
        string m_string;
        Val(int in, double d, char c , const string& str){
                m_int = in;
                m_double = d;
                m_char = c;
                m_string= str;
        }
        Val(){}
};

class DataVal{
public:
        ByteType m_type;
        Val m_val;
        DataVal(ByteType type, const Val& val){
                m_type = type;
                m_val = val;
        }
        DataVal(){}
};

class DataObj{
public:
        ByteType m_type;
        void* m_pointer;
        DataObj(ByteType type,void* pointer){
                m_type =type;
                m_pointer = pointer;
        }
};

class Local{
public:
        int m_depth;
        string m_name;
        DataVal m_val;
        Local(int depth, const string& name, DataVal val){
                m_depth = depth;
                m_val = val;
                m_name = name;
        }
};

class StructObj{
public:
        vector<Local*>* m_vals;
        map<string,string>* m_functions;
        string m_name;
        StructObj(string name,vector<Local*>* vals,map<string,string>* functions){
                m_functions = functions;
                m_name  = name;
                m_vals = new vector<Local*>();
                for(Local* local: *vals){
                        m_vals->push_back(new Local(0,local->m_name,local->m_val));
                }
        }

};

class StructDef{
        public:
                vector<Local*>* m_vals;
                map<string,string>* m_functions;
                string m_name;
                StructDef(string name,vector<Local*>* vals, map<string,string>* functions){
                        m_name = name;
                        m_vals = vals;
                        m_functions = functions;
                }
};

class Vm{
public:
        vector<DataVal> m_stack;   
        vector<string> m_functionStack;
        vector<DataObj*> m_objs;
        vector<ByteToken*> m_tokens;
        map<string,StructDef*> m_structs; 
        vector<vector<Local*>*> m_locals;
        vector<int> m_jumpBacks;
        map<string,int> m_labels;
        bool m_halted;
        bool m_hasError;
        int m_pos;
        vector<int> m_localCounts;
        Vm(vector<ByteToken*>& tokens);
        DataVal popStack();
        string popStackString();
        ByteToken* nextToken();
        void pushLocals(vector<Local*>* ins,vector<Local*>* outs);
        void newObj();
        void execute();
        void step();
        void println();
        void print();
        void pushToStack();
        void functionPush();
        void functionPop();
        void jump();
        void jumpIf();
        void dup();
        void load();
        void load(vector<Local*>* locals, const string& name);
        void store();
        void asi();
        void asi(vector<Local*>* locals);
        void call();
        void Return();
        void label();
        void append();
        void trig(ByteType type);
        void at();
        void del();
        void structDec();
        void structDecEx();
        void loadclass();
        void createLocal();
        void set();
        void select();
        void popLocal();
        void garbageCollect(int i);
        void collectAllGarbage();
        void input();
        void cast();
        void readFile();
        void writeFile();
        void classcall();
        void createFile();
        void deleteFile();
        void exists();
        void pop();
        void len();
        void binOP(ByteType type);
        void binOPDouble(ByteType type,double one,double two);
        void binOPSTRING(ByteType type, string one,string two);
        void runTimeError(string message);
};
#endif

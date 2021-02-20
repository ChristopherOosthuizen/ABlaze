

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

class Local{
public:
        int m_depth;
        int m_count;
        string m_name;
        DataVal m_val;
        Local(int depth, const string& name, DataVal val){
                m_depth = depth;
                m_val = val;
                m_name = name;
        }
};

class Vm{
public:
        vector<DataVal> m_stack;   
        vector<ByteToken*> m_tokens;
        vector<Local*> m_locals;
        map<string,DataVal> m_vars;
        map<string,int> m_labels;
        bool m_halted;
        int m_pos;
        int m_jumpBack;
        Vm(vector<ByteToken*>& tokens);
        void execute();
        void step();
        void print();
        void pushToStack();
        void jump();
        void jumpIf();
        void load();
        void store();
        void call();
        void Return();
        void label();
        void binOP(ByteType type);
        void binOPDouble(ByteType type,double one,double two);
        void binOPSTRING(ByteType type, string one,string two);
};
#endif

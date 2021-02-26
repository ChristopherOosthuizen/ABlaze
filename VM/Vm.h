

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
        DataObj(ByteType type){
                m_type =type;
                if(m_type == ByteType::LIST)
                        m_pointer = new vector<DataVal>();
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

class Vm{
public:
        vector<DataVal> m_stack;   
        vector<DataObj*> m_objs;
        vector<ByteToken*> m_tokens;
        vector<vector<Local*>*> m_locals;
        vector<int> m_jumpBacks;
        map<string,int> m_labels;
        bool m_halted;
        int m_pos;
        vector<int> m_localCounts;
        Vm(vector<ByteToken*>& tokens);
        void newObj();
        void execute();
        void step();
        void print();
        void pushToStack();
        void jump();
        void jumpIf();
        void load();
        void store();
        void asi();
        void call();
        void Return();
        void label();
        void append();
        void at();
        void del();
        void createLocal();
        void popLocal();
        void binOP(ByteType type);
        void binOPDouble(ByteType type,double one,double two);
        void binOPSTRING(ByteType type, string one,string two);
};
#endif

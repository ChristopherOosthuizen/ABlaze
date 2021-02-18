

#ifndef ABLAZE_VM_H
#define ABLAZE_VM_H
#include <vector>
#include <map>
#include <iostream>
#include "ByteToken.h"
using namespace std;
class Vm{
public:
        vector<int> m_stack;   
        vector<int> m_isString;
        vector<string> m_strings;
        vector<ByteToken*> m_tokens;
        map<string,int> m_vars;
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
};
#endif

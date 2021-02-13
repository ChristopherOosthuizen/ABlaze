

#ifndef ABLAZE_VM_H
#define ABLAZE_VM_H
#include <vector>
#include <map>
#include "ByteToken.h"
using namespace std;
class Vm{
public:
        vector<int> m_stack;        
        vector<ByteToken*> m_tokens;
        map<int,int> m_vars;
        bool m_halted;
        int m_pos;
        int m_jumpBack;
        Vm(vector<ByteToken*>& tokens);
        void execute();
        void step();
        void pushToStack();
        void jump();
        void jumpIf();
        void load();
        void store();
        void call();
        void Return();
        void binOP(ByteType type);
};
#endif

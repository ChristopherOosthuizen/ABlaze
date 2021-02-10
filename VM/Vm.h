

#ifndef ABLAZE_VM_H
#define ABLAZE_VM_H
#include <vector>
#include "ByteToken.h"
using namespace std;
class Vm{
public:
        vector<int> m_stack;        
        vector<ByteToken*> m_tokens;        
        bool m_halted;
        int m_pos;
        Vm(vector<ByteToken*>& tokens);
        void execute();
        void step();
        void pushToStack();
        void binOP(ByteType type);
};
#endif



#ifndef ABLAZE_BYTETOKEN_H
#define ABLAZE_BYTETOKEN_H
#include "ByteType.h"
#include <string>
using namespace std;
class ByteToken{
public:
        ByteType m_type;
        int m_value;
        string m_symbol;
        ByteToken(ByteType type,int value,const string& symbol){
                m_type =type;
                m_value =value;
                m_symbol = symbol;
        }
};
#endif

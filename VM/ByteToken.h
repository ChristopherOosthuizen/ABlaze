

#ifndef ABLAZE_BYTETOKEN_H
#define ABLAZE_BYTETOKEN_H
#include "ByteType.h"
class ByteToken{
public:
        ByteType m_type;
        int m_value;
        ByteToken(ByteType type,int value){
                m_type =type;
                m_value =value;
        }
};
#endif

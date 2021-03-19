#ifndef ABLAZE_BYTELEXER_H
#define ABLAZE_BYTELEXER_H 
#include "ByteToken.h"
#include <vector>
#include <map>
#include <string>
using namespace std;


class ByteLexer{
        string m_input;
        int m_pos;
        map<string,ByteType> m_types;
        string readNext();
        string strings(int start);
        string chars(int start);
        ByteToken* createToken(const string& str);
        ByteToken* numbers(const string& str);
        public:
                ByteLexer(const string& input);
                vector<ByteToken*> readAllTokens();


};
#endif

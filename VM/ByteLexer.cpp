
#include "ByteLexer.h"

ByteLexer::ByteLexer(const string& input){
        m_input = input+" ";
        m_pos =0;
        m_types["add"] = ByteType::ADD;
        m_types["divide"] = ByteType::DIVIDE;
        m_types["times"] = ByteType::TIMES;
        m_types["minus"] = ByteType::MINUS;
        m_types["halt"] = ByteType::HALT;
        m_types["load"] = ByteType::LOAD;
        m_types["store"] = ByteType::STORE;
        m_types["push"] = ByteType::PUSH;
}

string ByteLexer::readNext(){
        int start = m_pos;
        char c;
        do{
                c= m_input.at(m_pos++);
        }while(m_pos< m_input.size() && c != ' ' && c!='\n'); 
        return m_input.substr(start, m_pos-start-1);
}

ByteToken* ByteLexer::createToken(const string& str){
        if(m_types.count(str) !=0)
                return new ByteToken(m_types[str],0);
        return new ByteToken(ByteType::INT,stoi(str));
}

vector<ByteToken*> ByteLexer::readAllTokens(){
        vector<ByteToken*> result;
        while(m_pos <m_input.size()){
               string str = readNext(); 
               if(str.empty() || str ==" ")
                       continue;
               ByteToken* token = createToken(str);
               result.push_back(token);
        }
        return result;
}

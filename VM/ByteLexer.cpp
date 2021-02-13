
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
        m_types["xor"] = ByteType::XOR;
        m_types["not"] = ByteType::NOT;
        m_types["and"] = ByteType::AND;
        m_types["or"] = ByteType::OR;
        m_types["isgt"] = ByteType::ISGT;
        m_types["islt"] = ByteType::ISLT;
        m_types["isle"] = ByteType::ISLE;
        m_types["isge"] = ByteType::ISGE;
        m_types["jmp"] = ByteType::JMP;
        m_types["jif"] = ByteType::JIF;
        m_types["call"] = ByteType::CALL;
        m_types["return"] = ByteType::RETURN;

}

string ByteLexer::readNext(){
        int start = m_pos;
        char c;
        do{
                c= m_input.at(m_pos++);
        }while(m_pos< m_input.size() && c != ' ' && c!='\n'); 
        return m_input.substr(start, m_pos-start-1);
}

ByteToken* ByteLexer::createToken( const string& str){
        if(m_types.count(str) !=0)
                return new ByteToken(m_types[str],0,str);
        if(str.at(str.size()-1) == ':')
                return new ByteToken(ByteType::LABEL,0,str.substr(0,str.size()-1));
        if(isdigit(str.at(0)))
                return new ByteToken(ByteType::INT,stoi(str),str);
        return new ByteToken(ByteType::IDEN,0,str);
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

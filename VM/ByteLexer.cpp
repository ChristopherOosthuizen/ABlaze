
#include "ByteLexer.h"

ByteLexer::ByteLexer(const string& input){
        m_input = input+" ";
        m_pos =0;
        m_types["mod"] = ByteType::MOD;
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
        m_types["and_and"] = ByteType::AND_AND;
        m_types["or"] = ByteType::OR;
        m_types["or_or"] = ByteType::OR_OR;
        m_types["isgt"] = ByteType::ISGT;
        m_types["islt"] = ByteType::ISLT;
        m_types["isle"] = ByteType::ISLE;
        m_types["isge"] = ByteType::ISGE;
        m_types["jmp"] = ByteType::JMP;
        m_types["jif"] = ByteType::JIF;
        m_types["call"] = ByteType::CALL;
        m_types["return"] = ByteType::RETURN;
        m_types["println"] = ByteType::PRINTLN;
        m_types["print"] = ByteType::PRINT;
        m_types["startlocal"] = ByteType::CREATELOCAL;
        m_types["poplocal"] = ByteType::POPLOCAL;
        m_types["asi"] = ByteType::ASI;
        m_types["equal_equal"] = ByteType::EQUAL;
        m_types["new"] = ByteType::NEW;
        m_types["append"] = ByteType::APPEND;
        m_types["delete"] = ByteType::DELETE;
        m_types["at"] = ByteType::AT;
        m_types["structdecEx"] = ByteType::STRUCTCONSTEX;
        m_types["structdec"] = ByteType::STRUCTCONST;
        m_types["set"] = ByteType::SET;
        m_types["input"] = ByteType::INPUT;
        m_types["pow"] = ByteType::POW;
        m_types["sqrt"] = ByteType::SQRT;
        m_types["cast"] = ByteType::CAST;
        m_types["readFile"] = ByteType::READFILE;
        m_types["writeFile"] = ByteType::WRITEFILE;
        m_types["exists"] = ByteType::EXISTS;
        m_types["createFile"] = ByteType::CREATEFILE;
        m_types["deleteFile"] = ByteType::DELETEFILE;
        m_types["len"] = ByteType::LEN;
        m_types["loadclass"] = ByteType::LOADCLASS;
        m_types["classcall"] = ByteType::CLASSCALL;
        m_types["functionPush"] = ByteType::FUNCTIONPUSH;
        m_types["sin"] = ByteType::SINE;
        m_types["cos"] = ByteType::COS;
        m_types["tan"] = ByteType::TAN;
        m_types["left_shift"] = ByteType::LSHIFT;
        m_types["right_shift"] = ByteType::RSHIFT;
        m_types["nil"] = ByteType::NIL;
        m_types["neg"] = ByteType::NEG;
        m_types["dup"] = ByteType::DUP;
        m_types["pop"] = ByteType::POP;
        m_types["true"] = ByteType::TRUE;
        m_types["false"] = ByteType::FALSE;
        m_types["select"] = ByteType::SELECT;
}

string ByteLexer::strings(int start){
        m_pos--;
	char peeked = m_input.at(m_pos+1);
	while(m_pos < m_input.length() && (peeked != '"' )){ 
		m_pos++;
		peeked = m_input[m_pos+1]; 
	}
        m_pos+=2;
        return m_input.substr(start, (m_pos)-start);  

}

string ByteLexer::chars(int start){
        m_pos--;
	char peeked = m_input.at(m_pos+1);
	while(m_pos < m_input.length() && (peeked != '\'' )){ 
		m_pos++;
		peeked = m_input[m_pos+1]; 
	}
        m_pos+=2;
        return m_input.substr(start, (m_pos)-start);  


}

string ByteLexer::readNext(){
        int start = m_pos;
        char c;
        do{
                c= m_input.at(m_pos++);
                if(c== '"')
                        return strings(start);
        }while(m_pos< m_input.size() && c != ' ' && c!='\n'); 
        return m_input.substr(start, m_pos-start-1);
}

ByteToken* ByteLexer::createToken( const string& str){
        if(str.at(0) =='\'')
                return new ByteToken(ByteType::CHAR,0,str.substr(1,str.size()-2));
        if(str.at(0) =='"')
                return new ByteToken(ByteType::STRING,0,str.substr(1,str.size()-2));
        if(m_types.count(str) !=0)
                return new ByteToken(m_types[str],0,str);
        if(str.at(str.size()-1) == ':')
                return new ByteToken(ByteType::LABEL,0,str.substr(0,str.size()-1));
        if(str.at(0) == '.' ||isdigit(str.at(0)))
                return numbers(str);
        return new ByteToken(ByteType::IDEN,0,str);
}

ByteToken* ByteLexer::numbers(const string& str){
        ByteType type = ByteType::INT;
        for(int i=0; i <str.size();i++){
                if(str[i] == '.'){
                        type = ByteType::DOUBLE;
                        break;
                }
        }
        double  d= stod(str);
        return new ByteToken(type,d,str);
}

vector<ByteToken*> ByteLexer::readAllTokens(){
        vector<ByteToken*> result;
        while(m_pos <m_input.size()){
               string str = readNext(); 
               if(str.empty() || str ==" "||str =="\n")
                       continue;
               ByteToken* token = createToken(str);
               result.push_back(token);
        }
        return result;
}

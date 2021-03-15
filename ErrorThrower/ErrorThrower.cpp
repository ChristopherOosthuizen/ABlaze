// created by Christopher Oosthuizen
// on 12/31/20

#include "ErrorThrower.h"
#include <string>
#include <vector>
using namespace std;

bool ErrorThrower::hasError =false;
vector<string>* ErrorThrower::errors = new vector<string>();
string ErrorThrower::m_content = "";
void ErrorThrower::error(int line, string message){
	string mes = "Error: "+message+" on Line: "+to_string(line)+"\n";
	int i=0;
	int count =0;
	for(; i< m_content.size() && count < line-1;i++){
		if(m_content[i] =='\n')
			count++;
	}
	int end=i;
	for(; end < m_content.size() && m_content[end] !='\n';end++);
	string lineMessage =to_string(line)+" | "+m_content.substr(i,end-i);
	mes+=lineMessage;
	hasError = true;
	errors->push_back(mes);
	



}

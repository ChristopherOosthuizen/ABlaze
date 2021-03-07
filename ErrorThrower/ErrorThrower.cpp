// created by Christopher Oosthuizen
// on 12/31/20

#include "ErrorThrower.h"
#include <string>
#include <vector>
using namespace std;

bool ErrorThrower::hasError =false;
vector<string>* ErrorThrower::errors = new vector<string>();

void ErrorThrower::invalidToken(char symbol, int line){
	string  error = "Invalid Character symbol '" + to_string(symbol)+ "' on line "+ to_string(line);
	hasError = true;
	errors->push_back(error);
}

void ErrorThrower::doubleDot(int line){

	string error = "unreadable double on line: "+ to_string(line)+" becuase of multiple dots";
	hasError = true;

	errors->push_back(error);
}

void ErrorThrower::unterminatedString(int line){
	string error = "unterminated string starting on line "+ to_string(line);
	hasError = true;

	errors->push_back(error);

}


void ErrorThrower::missingSemiColon(int line){
	string error = "Error: missing semi colon on line "+ to_string(line);
	hasError = true;

	errors->push_back(error);

}

void ErrorThrower::illgalIdentifier(int line,string symbol){
	string error = "Error name:" +symbol +" on line: "+to_string(line) +" can not be used as a identifier because it is reserved"; 
	hasError = true;

	errors->push_back(error);
}

void ErrorThrower::unIntiazlizedVarible(int line,string symbol){
	string error = "Uniniazlized varible '"+symbol+"' on line: "+ to_string(line);
	hasError = true;

	errors->push_back(error);
}

void ErrorThrower::mismatchType(int line, string symbol, string type, string actual){

	string error = "Error: mismatch type in decleration with name: "+symbol+" on line: "+ to_string(line)+" Type declared with : "+(type)+" but actual value is: "+(actual);
	hasError = true;

	errors->push_back(error);
}

void ErrorThrower::unNamedError(const string& message, int line){
	string error = "Error: "+message +" on line: "+to_string(line+1);
	hasError = true;

	errors->push_back(error);

}

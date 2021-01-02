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


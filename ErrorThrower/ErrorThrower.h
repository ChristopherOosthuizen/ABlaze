/*
 * Created by Christopher Oosthuizen on 12/31/20
 * This class is designed to handle all error throwing
 * in a class 
 */

#ifndef  ABLAZE_ERRORTHROWER_H
#define ABLAZE_ERRORTHROWER_H

using namespace std;
#include <string>
#include <vector>

class ErrorThrower{
public:
	static bool hasError;
	static vector<string>* errors; //note varible gets deleted in the main function 
	static void invalidToken(char symbol, int line); // meant to be thrown when a token is not recognized
	static void doubleDot(int line); // thrown when a double has two dots
	static void unterminatedString(int line); // thrown when a string does not have a closing "
    static void missingSemiColon(int line);
	static void illgalIdentifier(int line);
};
#endif

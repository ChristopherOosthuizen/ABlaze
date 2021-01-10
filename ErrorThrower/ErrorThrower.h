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
	static void invalidToken(char symbol, int line);
	static void doubleDot(int line);
	static void unterminatedString(int line);
};
#endif

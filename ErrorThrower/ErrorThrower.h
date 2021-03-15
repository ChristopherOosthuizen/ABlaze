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
	static string m_content;
	static void error(int line,string message);
};
#endif

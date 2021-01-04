// Created by Christopher Oosthuizen on 12/31/20. 
// This is deigned to the test the Error Thrower classes
// functionality and its functionality within other classes

#include "gtest/gtest.h"

#include "ErrorThrower.h"
#include <string>
#include <vector>
/* 
 * checks to the invalidToken function
 * to see if it can report a error and add it to the errors list
 * as well as turn has Error to true. while also reporting the error
 * accurately
 */
TEST(ERRORS,invalidToken){
	ErrorThrower::invalidToken('@',12);	
	ASSERT_TRUE(ErrorThrower::hasError);

	ErrorThrower::hasError =false;
	delete ErrorThrower::errors ;
	ErrorThrower::errors = new vector<string>();	
}



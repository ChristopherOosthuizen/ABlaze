//Test that the smeatic analisis throws errors when required and not in correct sitiations

#include "gtest/gtest.h"
#include "ErrorThrower.h"
#include "SematicAn.h"
#include "ASTGen.h"
#include "Lexer.h"

//Make sure that the SematicAn throws errors on the use of reserved keyword
TEST(SematicAn, reservedKeywords){

	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	Lexer lexer("int print = 12; ");
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);

}

//Check to see if varibles used out of scope are error thrown
TEST(SematicAn, outOfScpoer){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	Lexer lexer("i =12; int i =13;");
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),1);
}
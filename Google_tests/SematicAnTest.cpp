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
	ASSERT_EQ(ErrorThrower::errors->size(),2);

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


//detemine wether the sematic can determine that varibles used within scope with structures are accurtly protrayed
TEST(Sematic,structures){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	Lexer lexer("int fith = 6;for(int i =0; i<12;i++){i =6; fith = 12;}");
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(!ErrorThrower::hasError);

} 

TEST(Sematic,structs){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	Lexer lexer("struct Pos{int x; int y;} int main(){Pos pos = new Pos; Position posses = new Pos;}");
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_EQ(ErrorThrower::errors->size(),1);


}


/*
//check to see if the sematic anlisis will throw a error if the wrong type of decleration is used on a varible
TEST(SematicAn,wrongUse){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	Lexer lexer("int i = 6.8; double it = 12; string itts = 12; bool bill= 5.3; string es = true;");
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),5);
}

//Test weather the Determine type functions can correctly deduce types
TEST(SematicAn,typer){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	Lexer lexer("6.8; 12; 12+6.8; 5+\"hello\"; true; \"hello\"+6.8");
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	ASSERT_EQ(an.endType(body->m_lines->at(0),nullptr,nullptr),TokenType::IDEN_DOUBLE);
	ASSERT_EQ(an.endType(body->m_lines->at(1),nullptr,nullptr),TokenType::IDEN_INT);
	ASSERT_EQ(an.endType(body->m_lines->at(2),nullptr ,nullptr),TokenType::IDEN_DOUBLE);
	ASSERT_EQ(an.endType(body->m_lines->at(3),nullptr,nullptr),TokenType::IDEN_STRING);
	ASSERT_EQ(an.endType(body->m_lines->at(4),nullptr,nullptr),TokenType::IDEN_BOOL);
	ASSERT_EQ(an.endType(body->m_lines->at(5),nullptr,nullptr),TokenType::IDEN_STRING);
}
*/

// test to see weather the ast can catch wrong typed functions and weather it can handle functions
TEST(Sematic,Functions){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string voidfunc = "void voider(){print(\"void func\");}\n";
	string stringfunc = "string roiders(){return \"hello\";}";
	string noReturn = "int loders(){ int i =0; println(12);}";
	string main = "int main(){roiders(); voiders(); noiders(); int i = roiders(); string hello = roiders();}";
	Lexer lexer(voidfunc+stringfunc+noReturn+main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),2);
	
}

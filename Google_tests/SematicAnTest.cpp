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
	Lexer lexer("void main(){int fith = 6;for(int i =0; i<12;i++){i =6; fith = 12;}}");
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
	Lexer lexer("struct Pos{int x; int y;} void main(){Pos pos = new Pos; Position posses = new Position; pos.t = 12;}");
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_EQ(ErrorThrower::errors->size(),2);
}




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
	ASSERT_EQ(ErrorThrower::errors->size(),2);
}

// test to see weather the ast can catch wrong typed functions and weather it can handle functions
TEST(Sematic,Functions){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string voidfunc = "void voider(){print(\"void func\");}\n";
	string stringfunc = "string roiders(){return \"hello\";}";
	string noReturn = "void loders(){ int i =0; println(12);}";
	string main = "void main(){roiders(); voiders(); noiders(); int i = roiders(); string hello = roiders();}";
	Lexer lexer(voidfunc+stringfunc+noReturn+main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),3);
	
}

// test to see weather the ast can catch wrong typed functions and weather it can handle functions
TEST(Sematic,paranLength){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "void type(int a, int b){println a+b;}void main(){ type(12,14); type(12);}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),1);
	
}

TEST(SematicAn,functionTypes){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "int read(){return 12;} void like(){println 12;} void main(){int i = read(); var o = like();}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),1);
	
}


TEST(SematicAn,functionCallTypes){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "int read(int i){return i;} int printer(12){return 1;} void main(){read(\"hello\");}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),2);
	
}

TEST(SematicAn,returnCheck){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "int read(int i){if(i == 12){return 5.8;}} void right(){return 3;} void main(){}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),3);
	
}

TEST(SematicAn,enforceBool){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "void main(){ if(\"hello\"){println \"hello\";} while(12.3){} for(;4.5;){println 14;} }";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),3);
	
}

TEST(SematicAn,listCatch){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "void main(){ var s = int[]; string: r = string[]; int i = r[i]; string ss = \"hello\"; ss[0] = \"hello\";}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),3);
	
}


TEST(SematicAn,listFunctions){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "int: funcs(){return {12,14};} void main(){ int: lister = funcs(); int i =funcs();}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),1);
	
}

TEST(SematicAn, listLiteral){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "void main(){ int: lister = {12,5.5,16,18}; int: i ={14,5,6};}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),1);
	
}

TEST(SematicAn, paramterArray){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "void make(int: a, char: o){println 1;}void main(){ make({12,5.4,1},{'a','b'});}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),1);
	
}

TEST(SematicAn, checkFunctionAppend){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "void main(){int: i = {12,15,18,19}; append(i,5.8);}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),1);
	
}

TEST(SematicAn, classeinarrays){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "struct Person{int age; string name; void Person(int age, string name){this.age = age; this.name = name;}} void main(){ Person: ps= Person[]; append(ps, new Person(12,\"hello\")); }";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(!ErrorThrower::hasError);
	
}

TEST(SematicAn, builtinerror){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "void main(){int i = println(12); int i = len(12);}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),1);
	
}




TEST(SematicAn, classFuncs){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "struct Pos{ void read(){} double make(){return \"12\";} } void main(){ Pos pos = new Pos; pos.read(); int i= pos.make(); pos.right();}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),3);
	
}

TEST(SematicAn, strucElements){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "struct Pos{ double x =12; double y = 14; } void main(){ Pos pos = new Pos; int i = pos.x;  pos.z;}";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(ErrorThrower::hasError);
	ASSERT_EQ(ErrorThrower::errors->size(),2);
	
}



TEST(SematicAn, structsFunctions){
	ErrorThrower::hasError = false;
	delete ErrorThrower::errors;
	ErrorThrower::errors = new vector<string>();
	string main = "struct Pos{ void make(){} void write(){ make();}} void main(){ }";
	Lexer lexer(main);
	ASTGen gen = ASTGen(lexer.readAllTokens());
	Body* body = gen.generateAST();	
	SematicAn an(body);
	an.analize();
	ASSERT_TRUE(!ErrorThrower::hasError);
	
}



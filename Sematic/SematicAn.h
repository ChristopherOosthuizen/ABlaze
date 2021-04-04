// Anlize the Abstract sytntax tree using sematics.

#ifndef ABLAZE_SematicAn_h
#define ABLAZE_SematicAn_h
#include "AST.h"
#include <map>
#include "Lexer.h"

class TypeInfo{
	public:
		string m_symbol;
		bool m_isArray;
		bool m_map;
		TokenType m_mapType;
		TokenType m_type;
		TypeInfo(string symbol,TokenType type, bool isArray ){
			m_isArray = isArray;
			m_symbol = symbol;
			m_type = type;
			m_map = false;
		}
		TypeInfo(string symbol,TokenType type, bool isArray ,bool maper, TokenType mapType){
			m_isArray = isArray;
			m_symbol = symbol;
			m_type = type;
			m_map = maper;
			m_mapType = mapType;
		}

		TypeInfo(){}
};
class FunctionInfo{
	public:
		TypeInfo m_info;
		vector<TypeInfo> m_args;
		string m_name;
		int m_level;
		bool m_static;
		bool m_public;
		FunctionInfo(int level, string name, TypeInfo info,vector<TypeInfo> args){
			m_level = level;
			m_name = name;
			m_info = info;
			m_args = args;
		}
		FunctionInfo(bool pub,bool stat, int level, string name, TypeInfo info,vector<TypeInfo> args){
			m_level = level;
			m_static = stat;
			m_public = pub;
			m_name = name;
			m_info = info;
			m_args = args;
		}
		FunctionInfo(){}
};
class Lock{
	public:
		int m_level;
		string m_symbol;
		TypeInfo m_type;
		Lock(int level, string symbol, TypeInfo type){
			m_level = level;
			m_symbol = symbol;
			m_type = type;
		}
		Lock(){}
};

class SematicAn{
	Body* m_body;
	vector<Lock> m_vars;
	vector<FunctionInfo> m_functions;
	map<string,vector<FunctionInfo>> m_structsFunctions;
	map<string,map<string,TypeInfo>> m_structs;
	int m_level;
	string m_inside;
	public:
		SematicAn(Body* body);
		void analize();
		bool containsFunc(vector<FunctionInfo> info,string func);
		FunctionInfo getFunc(vector<FunctionInfo> info,string func);
		void check(Expression* expression);
		void checkFunction(Body* body);
		void checkBinOP(BinOP* op);
		void checkBody(Body* body);
		bool checkReturns(Body* body,TypeInfo type);
		TypeInfo getTypeBin(BinOP* bin);
		TypeInfo getType(Expression* expression);
		TypeInfo getTypeDot(Dot* dot);
		int score(TokenType type);
		void checkTypeEquality(int line,TypeInfo one, TypeInfo two);
		void checkDecleration(Decleration* dec);
		void checkFunctionCall(FunctionCall* call);
		void checkStructs(Body* body);
		void checkDot(Dot* dot);
		void checkArrayLiteral(ArrayLiteral* literal);
		void checkLiteral(Literal* literal);
		bool containsVar(string str);
		Lock getVar(string str);
		void popLevel();
		void increaseLevel();
		void checkForArray(ForArray* array);
		bool isReserved(const string& s);
		void checkFunction(Function* function);
		void controlStatements( Expression* expr);
		int getLine(Expression* expression);
			
};
#endif

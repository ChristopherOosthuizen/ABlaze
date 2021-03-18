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
		TokenType m_type;
		TypeInfo(string symbol,TokenType type, bool isArray){
			m_isArray = isArray;
			m_symbol = symbol;
			m_type = type;
		}
		TypeInfo(){}
};
class FunctionInfo{
	public:
		TypeInfo m_info;
		vector<TypeInfo> m_args;
		FunctionInfo(TypeInfo info,vector<TypeInfo> args){
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
	map<string,FunctionInfo> m_functions;
	map<string,map<string,TypeInfo>> m_structsFunctions;
	map<string,map<string,TypeInfo>> m_structs;
	int m_level;
	public:
		SematicAn(Body* body);
		void analize();
		void check(Expression* expression);
		void checkFunction(Body* body);
		void checkBinOP(BinOP* op);
		void checkBody(Body* body);
		TokenType getTypeBin(BinOP* bin);
		TokenType getType(Expression* expression);
		int score(TokenType type);
		void checkTypeEquality(int line,TokenType one, TokenType two);
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
		bool isReserved(const string& s);
		void checkFunction(Function* function);
		void controlStatements(Expression* expr);
			
};
#endif

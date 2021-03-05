// Anlize the Abstract sytntax tree using sematics.

#ifndef ABLAZE_SematicAn_h
#define ABLAZE_SematicAn_h
#include "AST.h"
#include <map>
#include "Lexer.h"

class Lock{
	public:
		int m_level;
		string m_symbol;
		TokenType m_type;
		Lock(int level, string symbol, TokenType type){
			m_level = level;
			m_symbol = symbol;
			m_type = type;
		}
};

class SematicAn{
	Body* m_body;
	vector<Lock> m_vars;
	map<string,TokenType> m_functions;
	int m_level;
	public:
		SematicAn(Body* body);
		void analize();
		void check(Expression* expression);
		void checkFunction(Body* body);
		void checkBinOP(BinOP* op);
		void checkBody(Body* body);
		void checkDecleration(Decleration* dec);
		void checkFunctionCall(FunctionCall* call);
		void checkDot(Dot* dot);
		void checkArrayLiteral(ArrayLiteral* literal);
		void checkLiteral(Literal* literal);
		bool containsVar(string str);
		void popLevel();
		void increaseLevel();
		void checkFunction(Function* function);
		void controlStatements(Expression* expr);
			
};
#endif

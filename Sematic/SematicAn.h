// Anlize the Abstract sytntax tree using sematics.

#ifndef ABLAZE_SematicAn_h
#define ABLAZE_SematicAn_h
#include "AST.h"
#include <map>
#include "Lexer.h"

class SematicAn{
	Body* m_body;
	public:
		SematicAn(Body* body);
		void analize();
		void checkVaribles(Body* body,map<string,TokenType>* variables);
		TokenType endType(Expression* expr, map<string,TokenType>* vars);    
		void addFor(ForStat* stat,map<string,TokenType>* vars);
		void addFunc(Function* func, map<string,TokenType>* vars);
};
#endif

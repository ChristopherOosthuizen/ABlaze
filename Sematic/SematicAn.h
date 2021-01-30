// Anlize the Abstract sytntax tree using sematics.

#ifndef ABLAZE_SematicAn_h
#define ABLAZE_SematicAn_h
#include "AST.h"
class SematicAn{
	Body* m_body;
	public:
		SematicAn(Body* body);
		void analize();
		void checkForReservedKeywords(Body* body);
    
};
#endif

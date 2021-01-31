//The purpose of this file is to symatically analize a AST

#include "SematicAn.h"
#include "ErrorThrower.h"


SematicAn::SematicAn(Body* body){
	m_body  = body;
}

void SematicAn::analize(){
	checkForReservedKeywords(m_body);
}


//Check to make sure that not decleration is using a reserved keyword
void SematicAn::checkForReservedKeywords(Body* body){
	vector<Expression*>* expressions = body->m_lines;
	for(Expression* expr : *(expressions)){
		if(expr->name() == "Body"){
			checkForReservedKeywords((Body*)expr);
		}else if(expr->name() == "Decleration"){
			Decleration* dec = (Decleration*)expr;
			TokenType type;
			int line;
			if(dec->m_name->name() == "ArrayLiteral"){
					type = ((ArrayLiteral*)dec->m_name)->m_iden->m_token->m_type;
					line = ((ArrayLiteral*)dec->m_name)->m_iden->m_token->m_line;
			}else{
				type = ((Literal*)dec->m_name)->m_token->m_type;
				line = ((Literal*)dec->m_name)->m_token->m_line;
			}
		if(type != TokenType::IDEN){
			ErrorThrower::illgalIdentifier(line);

		}
	}
}
}

//check to see if varibles are declared and idntified correctly
void SematicAn::checkVaribles(Body* body, map<string,TokenType>* variables){
	map<string,TokenType>* varibles = new map<string,TokenType>();
	for(Expression* expr: *(body->m_lines)){
		if(expr->name() == "Decleration"){
			Decleration* dec = (Decleration*)expr;
			string name;
			TokenType type;
			int line;
			if(dec->m_name->name() == "ArrayLiteral"){
					type = ((ArrayLiteral*)dec->m_name)->m_iden->m_token->m_type;
					name = ((ArrayLiteral*)dec->m_name)->m_iden->m_token->m_symbol;
					line = ((ArrayLiteral*)dec->m_name)->m_iden->m_token->m_line;
			}else{
				type = ((Literal*)dec->m_name)->m_token->m_type;
				name = ((Literal*)dec->m_name)->m_token->m_symbol;
				line = ((Literal*)dec->m_name)->m_token->m_line;

			}
			if(dec->m_initalize){
				(*variables)[name] =type; 
			}else {
				if(!variables->count(name)){
					ErrorThrower::unIntiazlizedVarible(line);
				}
			}
		}
	}

}

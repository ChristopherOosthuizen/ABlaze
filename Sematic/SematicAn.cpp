//The purpose of this file is to symatically analize a AST

#include "SematicAn.h"
#include "ErrorThrower.h"


SematicAn::SematicAn(Body* body){
	m_body  = body;
}

void SematicAn::analize(){
	checkForReservedKeywords(m_body);
	checkVaribles(m_body,nullptr);
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
void SematicAn::checkVaribles(Body* body, map<string,TokenType>* Outervariables){
	map<string,TokenType>* variables = new map<string,TokenType>();
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

//Determine the token type through a function
TokenType SematicAn::endType(Expression* expr,map<string,TokenType>* vars){
	if(expr->name() == "Literal"){
		TokenType type =((Literal*) expr)->m_token->m_type; 
		if(type != TokenType::IDEN)
			return ((Literal*) expr)->m_token->m_type;
		string name =((Literal*)expr)->m_token->m_symbol ;
		TokenType str = (*vars)[name]; 
		return str; 
	}
	BinOP* oper = (BinOP*)expr;
	TokenType left = endType(oper->m_left,vars);
	TokenType right = endType(oper->m_right,vars);
	if(left == TokenType::STRING || right == TokenType::STRING){
		return TokenType::STRING;
	}
	if(left == TokenType::DOUBLE || right == TokenType::DOUBLE){
		return TokenType::DOUBLE;
	
	}
	if(left == TokenType::INT || right == TokenType::INT ){
		return TokenType::INT;
	
	}
	return TokenType::BOOL;
}

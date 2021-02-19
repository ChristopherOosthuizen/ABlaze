//The purpose of this file is to symatically analize a AST

#include "SematicAn.h"
#include "ErrorThrower.h"


SematicAn::SematicAn(Body* body){
	m_body  = body;
}

void SematicAn::analize(){
	checkVaribles(m_body,nullptr,nullptr);
}

//check to see if varibles are declared and idntified correctly
void SematicAn::checkVaribles(Body* body, map<string,TokenType>* Outervariables, map<string,TokenType>* outFunctions){
	map<string,TokenType>* functions = new map<string,TokenType>();
	map<string,TokenType>* variables = new map<string,TokenType>();
	if(Outervariables != nullptr)
		variables->insert(Outervariables->begin(), Outervariables->end());
	if(outFunctions!= nullptr)
		functions->insert(variables->begin(), variables->end());

	for(Expression* expr: *(body->m_lines)){
		if(expr->name() == "Body"){
			string typer =((Body*)expr)->m_control->name() ; 
			if(typer == "For")
				addFor((ForStat*)((Body*)expr)->m_control,variables);	
			else if(typer == "Function")
				(*functions)[((Function*)((Body*)expr)->m_control)->m_call->m_name->m_token->m_symbol] = ((Function*)expr)->m_type->m_token->m_type; 	
			checkVaribles((Body*)expr,variables,functions);
		}
		if(expr->name() == "Decleration"){
			checkDecleration((Decleration*)expr,variables,functions);
	}
	}
        delete functions;
        delete variables;

}

//Check that a decleration was declared and used properly
void SematicAn::checkDecleration(Decleration* dec,map<string,TokenType>* vars,map<string,TokenType>* functions){
			TokenType type;
			TokenType actual = endType(dec->m_value,vars,functions);
			Token* token ; 

			if(dec->m_name->name() == "ArrayLiteral"){
					type = ((ArrayLiteral*)dec->m_type)->m_iden->m_token->m_type;
					token =((ArrayLiteral*)dec->m_name)->m_iden->m_token; 
			}else{
				type = ((Literal*)dec->m_type)->m_token->m_type;
				token =((Literal*)dec->m_name)->m_token;	
			}


			if(token->m_type != TokenType::IDEN){
				ErrorThrower::illgalIdentifier(token->m_line,token->m_symbol);

			}

			if(dec->m_initalize){
				(*vars)[token->m_symbol] =type; 
			}else {
				if(!vars->count(token->m_symbol)){
					ErrorThrower::unIntiazlizedVarible(token->m_line,token->m_symbol);
				}
				type = (*vars)[token->m_symbol];
				return;
			}
			if(actual != type ){
				ErrorThrower::mismatchType(token->m_line,token->m_symbol,Lexer::typeToString(type),Lexer::typeToString(actual));
			}
		
}

//Determine the token type through a function
TokenType SematicAn::endType(Expression* expr,map<string,TokenType>* vars ,map<string,TokenType>* funcs){
	if(expr == nullptr)
		return TokenType::VOID;
	
	if(expr->name() == "Literal"){
		return endTypeLiteral((Literal*)expr,vars,funcs);
	}else if(expr->name() == "FunctionCall"){
		return endTypeFunctionCall((FunctionCall*)expr,vars,funcs);
	}
	BinOP* oper = (BinOP*)expr;
	TokenType left = endType(oper->m_left,vars,funcs);
	TokenType right = endType(oper->m_right,vars,funcs);
	if(left == TokenType::IDEN_STRING || right == TokenType::IDEN_STRING){
		return TokenType::IDEN_STRING;
	}
	if(left == TokenType::IDEN_DOUBLE || right == TokenType::IDEN_DOUBLE){
		return TokenType::IDEN_DOUBLE;
	
	}
	if(left == TokenType::IDEN_INT || right == TokenType::IDEN_INT ){
		return TokenType::IDEN_INT;
	
	}

	return TokenType::IDEN_BOOL;
}

//Return the type of a expression if its a literal
TokenType SematicAn::endTypeLiteral(Literal* expr,map<string,TokenType>* vars, map<string,TokenType>* funcs){
		TokenType type =expr->m_token->m_type; 
		if( type != TokenType::IDEN){
			switch(((Literal*) expr)->m_token->m_type){
				case TokenType::DOUBLE: return TokenType::IDEN_DOUBLE;
				case TokenType::BOOL: return TokenType::IDEN_BOOL;
				case TokenType::STRING: return TokenType::IDEN_STRING;
				case TokenType::INT: return TokenType::IDEN_INT;
                                case TokenType::CHAR: return TokenType::IDEN_CHAR;

			}
			return ((Literal*) expr)->m_token->m_type;
		}
		if(type == TokenType::IDEN){
			return TokenType::IDEN_INT;
		}
		return TokenType::IDEN_BOOL; 
		
}

//return the type of a function call
TokenType SematicAn::endTypeFunctionCall(FunctionCall* expr, map<string,TokenType>* vars,map<string,TokenType>* funcs){
		string name =expr->m_name->m_token->m_symbol; 
		if(funcs->count(name) ==0){
			int line = ((FunctionCall*)expr)->m_name->m_token->m_line;
			ErrorThrower::unIntiazlizedVarible(line,name);

			return TokenType::VOID;	
		}
		return (*funcs)[name];	

}

void SematicAn::addFor(ForStat* stat, map<string,TokenType>* vars){
	Expression* initial = stat->m_initial;
	if(initial->name() == "Decleration"){
		Decleration* dec = (Decleration*)initial;
		Literal* name= (Literal*)dec->m_name;
		Literal* type = dec->m_type;
		(*vars)[name->m_token->m_symbol] = type->m_token->m_type;
	}
}

//The purpose of this file is to symatically analize a AST

#include "SematicAn.h"
#include "ErrorThrower.h"


SematicAn::SematicAn(Body* body){
	m_body  = body;
	m_level =0;
	m_functions["delete"] = TokenType::VOID;
	m_functions["input"] = TokenType::STRING;
	m_functions["append"] = TokenType::VOID;
	m_functions["deleteFile"]= TokenType::VOID;
	m_functions["createFile"]= TokenType::VOID;
	m_functions["exists"]= TokenType::INT;
	m_functions["writeFile"]= TokenType::VOID;
	m_functions["readFile"]= TokenType::LIST;
}

void SematicAn::analize(){
	for(int i=0; i< m_body->m_lines->size(); i++){
		if(m_body->m_lines->at(i)->name() == "Body"){
			Body* body = (Body*)m_body->m_lines->at(i); 
			controlStatements(body->m_control);
		}	
	}
	check(m_body);
}

void SematicAn::check(Expression* expr){
	if(expr == NULL)
		return;
	string name = expr->name(); 
	if(name == "Decleration"){
		checkDecleration((Decleration*) expr);
	}else if(name == "BinOP"){
		checkBinOP((BinOP*) expr);
	}else if(name == "Body"){
		checkBody((Body*) expr);
	}else if(name == "FunctionCall"){
		checkFunctionCall((FunctionCall*)expr);
	}else if(name == "Dot"){
		checkDot((Dot*)expr);
	}else if(name =="ArrayLiteral"){
		checkArrayLiteral((ArrayLiteral*) expr);
	}else if(name == "Literal"){
		checkLiteral((Literal*)expr);
	}else if(name == "BuiltIn"){
		BuiltIn* ins = (BuiltIn*)expr;
		if(ins->m_type->m_token->m_type != TokenType::NEW)
			check(ins->m_value);
	}
}

void SematicAn::increaseLevel(){
	m_level++;
}


void SematicAn::popLevel(){
	while(m_vars.size() >0 && m_vars[m_vars.size()-1].m_level == m_level){
		m_vars.pop_back();
	}
	m_level--;
}

bool SematicAn::containsVar(string str){
	for(int i=0; i<m_vars.size();i++){
		if(m_vars[i].m_symbol == str)
			return true;
	}
	return false;
}


void SematicAn::checkLiteral(Literal* literal){
	Token* token = literal->m_token;	
	if(token->m_type == TokenType::IDEN){
		if(!containsVar(token->m_symbol)){
			ErrorThrower::unIntiazlizedVarible(token->m_line,token->m_symbol);
		}
	}
}


void SematicAn::checkDot(Dot* dot){}
void SematicAn::checkBinOP(BinOP* binop){
	check(binop->m_right);
	check(binop->m_left);
}

void SematicAn::checkBody(Body* body){
	increaseLevel();
	if(body->m_control != nullptr &&body->m_control->name() == "Struct"){
		checkStructs(body);
		return;
	}
	controlStatements(body->m_control);
	for(int i=0; i< body->m_lines->size(); i++){
		check(body->m_lines->at(i));
	}
	popLevel();
}

void SematicAn::checkStructs(Body* body){
	Struct* control = (Struct*)body->m_control;
	string name = control->m_iden->m_token->m_symbol;
	map<string,TokenType> vars;
	for(int i=0; i< body->m_lines->size(); i++){
		Expression* expr =body->m_lines->at(i); 
		check(expr);
		if(expr->name() != "Decleration"){
			ErrorThrower::unNamedError("Non decleration in struct",control->m_iden->m_token->m_line);
			continue;
		}
		Decleration* dect = (Decleration*)expr;
		if(!dect->m_initalize){
			ErrorThrower::unNamedError("Loose variable calls are illigal in structs ",control->m_iden->m_token->m_line);
			continue;
		}
		else if(dect->m_value != nullptr){
			ErrorThrower::unNamedError("Variables can not have values in struct",control->m_iden->m_token->m_line);
			continue;
		}
		string name = ((Literal*)dect->m_name)->m_token->m_symbol;
		TokenType type =dect->m_type->m_token->m_type; 
		vars[name] = type;
		
	}
	m_structs[name] = vars;
	popLevel();
}

void SematicAn::controlStatements(Expression* expression){
	if(expression == nullptr)
		return;
	string name = expression->name();
	if(name == "For"){
		ForStat* stat = (ForStat*)expression;
		check(stat->m_initial);
		check(stat->m_condition);
		check(stat->m_repitition);
	}else if(name=="If"){
		IfStat* stat = (IfStat*)expression;
		check(stat->m_control);
	}else if(name == "While"){
		IfStat* stat = (IfStat*)expression;
		check(stat->m_control);
	}else if(name == "Function"){
		checkFunction((Function*) expression);
	}
}

void SematicAn::checkFunction(Function* expr){
	m_functions[expr->m_call->m_name->m_token->m_symbol] =  expr->m_type->m_token->m_type;
	checkFunctionCall(expr->m_call);
}


void SematicAn::checkFunctionCall(FunctionCall* functionCall){
	Token* token = functionCall->m_name->m_token; 
	string name =token->m_symbol;  
	if(m_functions.count(name) ==0 )
		ErrorThrower::unIntiazlizedVarible(token->m_line,name);
	for(int i=0; i< functionCall->m_args->size(); i++){
		check(functionCall->m_args->at(i));
	}
}

void SematicAn::checkDecleration(Decleration* decleration){
	
	Token* name = ((Literal*)decleration->m_name)->m_token;
	if(decleration->m_initalize){
		Token* type = decleration->m_type->m_token;

		m_vars.push_back(Lock(m_level,name->m_symbol,type->m_type));

		if(type->m_type ==TokenType::IDEN){
			if(m_structs.count(type->m_symbol) ==0){
				ErrorThrower::unNamedError("Unkown type",type->m_line);
			}
		}
	}else{
		check(decleration->m_name);

	}
	check(decleration->m_value);
}

void SematicAn::checkArrayLiteral(ArrayLiteral* literal){}
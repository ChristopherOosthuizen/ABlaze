//The purpose of this file is to symatically analize a AST

#include "SematicAn.h"
#include "ErrorThrower.h"


SematicAn::SematicAn(Body* body){
	m_body  = body;
	m_level =0;
	string argsMes = " with args count ";
	vector<TypeInfo> info  ={TypeInfo("List",TokenType::VAR,true), TypeInfo("int",TokenType::INT,false)};
	m_functions["delete"+argsMes+"2"] = FunctionInfo(TypeInfo("void",TokenType::VOID,false),info );
	info  ={};
	m_functions["input"+argsMes+"0"] = FunctionInfo(TypeInfo("string",TokenType::STRING,false),info );
	info  ={TypeInfo("List",TokenType::VAR,true), TypeInfo("int",TokenType::INT,false)};
	m_functions["append"+argsMes+"2"] = FunctionInfo(TypeInfo("void",TokenType::VOID,false),info );
	info  ={TypeInfo("string",TokenType::STRING,false)};
	m_functions["deleteFile"+argsMes+"1"]= FunctionInfo(TypeInfo("void",TokenType::VOID,false),info );
	m_functions["createFile"+argsMes+"1"]= FunctionInfo(TypeInfo("void",TokenType::VOID,false),info );
	m_functions["exists"+argsMes+"1"]= FunctionInfo(TypeInfo("int",TokenType::INT,false),info );
	info  ={TypeInfo("string",TokenType::STRING,false),TypeInfo("string",TokenType::STRING,false)};
	m_functions["writeFile"+argsMes+"2"]= FunctionInfo(TypeInfo("void",TokenType::VOID,false),info );
	info  ={TypeInfo("string",TokenType::STRING,false)};
	m_functions["readFile"+argsMes+"1"]= FunctionInfo(TypeInfo("string",TokenType::LIST,true),info );
	info  ={TypeInfo("double",TokenType::DOUBLE,false)};
	m_functions["sin"+argsMes+"1"]= FunctionInfo(TypeInfo("double",TokenType::DOUBLE,true),info );
	m_functions["cos"+argsMes+"1"]= FunctionInfo(TypeInfo("double",TokenType::DOUBLE,true),info );
	m_functions["tan"+argsMes+"1"]= FunctionInfo(TypeInfo("double",TokenType::DOUBLE,true),info );
}

TokenType SematicAn::getTypeBin(BinOP*  op){
	TokenType os = op->m_op->m_type;
	switch(os){
		case TokenType::TIMES_TIMES:
		case TokenType::DIVIDE_DIVIDE:return TokenType::DOUBLE;
		case TokenType::OR_OR:
		case TokenType::AND_AND:
		case TokenType::LESS:
		case TokenType::GREATER:
		case TokenType::MORE_EQUAL:
		case TokenType::LESS_EQUAL:return TokenType::BOOL;
	}
	TokenType left = getType(op->m_left);
	TokenType right = getType(op->m_right);
	if(left == TokenType::STRING || right == TokenType::STRING){
		return TokenType::STRING;
	}else if(left == TokenType::DOUBLE || right == TokenType::DOUBLE){
		return TokenType::DOUBLE;
	}else if(left == TokenType::INT || right == TokenType::INT){
		return TokenType::INT;
	}else if(right == TokenType::CHAR ||left == TokenType::CHAR ){
		return TokenType::INT;
	}else if(right == TokenType::BOOL || left == TokenType::BOOL){
		return TokenType::INT;
	}
	return TokenType::VAR;
}

TokenType SematicAn::getType(Expression* expression){
	if(expression == nullptr)
		return TokenType::VAR;
	if(expression->name() == "BinOP"){
		return  getTypeBin((BinOP*)expression);
	}else if(expression->name() == "Cast"){
		return ((Cast*)expression)->m_iden->m_token->m_type;
	}else if(expression->name() == "Literal"){
		return ((Literal*)expression)->m_token->m_type;
	}else if(expression->name() == "FunctionCall"){
		FunctionCall* call = (FunctionCall*)expression;
		string name = call->m_name->m_token->m_symbol +" with args count "+to_string(call->m_args->size());
		return m_functions[name].m_info.m_type;
	}
	return TokenType::VAR;
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
	if(expr == nullptr)
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
		else{
			string symbol = "";
			int line = 0;
			if(ins->m_value->name() == "Literal"){
				Literal* lit = (Literal*)ins->m_value;
				symbol = lit->m_token->m_symbol;
				line = lit->m_token->m_line;
			}
			else if(ins->m_value->name() == "FunctionCall"){
				FunctionCall* call = (FunctionCall*)ins->m_value;
				symbol = call->m_name->m_token->m_symbol;
				line = call->m_name->m_token->m_line;
			}else{
				return;
			}
			if(m_structs.count(symbol) == 0){

				ErrorThrower::error(line,"Undefinded class refrence" );
			}
		}
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

Lock SematicAn::getVar(string str){
	for(int i=0; i<m_vars.size();i++){
		if(m_vars[i].m_symbol == str)
			return m_vars[i];
	}
	return Lock();
}

bool SematicAn::isReserved(const string& s){
	vector<string> strs = {"this"};
	for(string st : strs){
		if(st == s)
			return true;
	}
	return false;
}

void SematicAn::checkLiteral(Literal* literal){
	Token* token = literal->m_token;	
	if(token->m_type == TokenType::IDEN){
		if(!containsVar(token->m_symbol) &&!isReserved(token->m_symbol) ){
			ErrorThrower::error(token->m_line,"uninitilized variable: "+token->m_symbol);
		}
	}
}


void SematicAn::checkDot(Dot* dot){
	check(dot->m_iden);	
	/*if(containsVar(dot->m_iden->m_token->m_symbol)){
		string type = getVar(dot->m_iden->m_token->m_symbol).m_type.m_symbol;
		Token* sub = dot->m_subIden->m_token;
		if(m_structs[type].count(sub->m_symbol) ==0){
			ErrorThrower::unNamedError("unkown type",sub->m_line);
		}
	}*/
}


void SematicAn::checkBinOP(BinOP* binop){
	check(binop->m_right);
	check(binop->m_left);
}

bool SematicAn::checkReturns(Body* body,TokenType type){
	bool hasReturn = false;
	for(Expression* expr: *body->m_lines){
		if(expr->name() == "BuiltIn"){
			BuiltIn* name = ((BuiltIn*)expr);
			if(name->m_type->m_token->m_type == TokenType::RETURN){
				if(type != TokenType::VOID)
					checkTypeEquality(name->m_type->m_token->m_line,type,getType(name->m_value));
				else if(name->m_value != nullptr)
					ErrorThrower::error(name->m_type->m_token->m_line, "use of non null return in void function");
					
				hasReturn = true;
			}
		}else if(expr->name() == "Body"){
			checkReturns((Body*)expr,type);
		}
	}
	return hasReturn;
}

void SematicAn::checkBody(Body* body){
	increaseLevel();
	if(body->m_control != nullptr &&body->m_control->name() == "Struct"){
		checkStructs(body);
		return;
	}
	if(body->m_control != nullptr &&body->m_control->name() == "Function"){
		Function* func = (Function*)body->m_control;
		TokenType type =func->m_type->m_token->m_type; 
		bool hasRet = checkReturns(body,type);
		if(!hasRet && type != TokenType::VOID)
			ErrorThrower::error(func->m_type->m_token->m_line,"Function can reach end without return");
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

	map<string,TypeInfo> vars;
	m_structs[name] = vars;

	for(int i=0; i< body->m_lines->size(); i++){
		Expression* expr =body->m_lines->at(i); 
		check(expr);
		if(expr->name() == "Decleration"){
		
			Decleration* dect = (Decleration*)expr;
		
			string name = ((Literal*)dect->m_name)->m_token->m_symbol;
			Token* type =dect->m_type->m_token;
			vars[name] =  TypeInfo(type->m_symbol,type->m_type, dect->m_isArray);
		}
		
	}
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
	Token* token =expr->m_type->m_token; 
	FunctionCall* call =expr->m_call; 
	string name =call->m_name->m_token->m_symbol+" with args count "+to_string(call->m_args->size());  
	vector<TypeInfo> args; 
	for(Expression* expression:*call->m_args){
		if(expression->name() != "Decleration"){
			ErrorThrower::error(token->m_line,"args must be declerations");
			args.push_back(TypeInfo("",TokenType::NIL,false));
			continue;
		}
		Decleration* arg = (Decleration*)expression;
		if(!arg->m_initalize){
			ErrorThrower::error(token->m_line,"args must be declerations");
			args.push_back(TypeInfo("",TokenType::NIL,false));
			continue;
	
		}
		args.push_back(TypeInfo(arg->m_type->m_token->m_symbol,arg->m_type->m_token->m_type,arg->m_isArray));

	}
	m_functions[name] =  FunctionInfo(TypeInfo(token->m_symbol,token->m_type,false),args);
	checkFunctionCall(expr->m_call);
}


void SematicAn::checkFunctionCall(FunctionCall* functionCall){
	Token* token = functionCall->m_name->m_token; 
	string name =token->m_symbol+" with args count "+to_string(functionCall->m_args->size());  
	if(m_functions.count(name) ==0 ){
		ErrorThrower::error(token->m_line, "Undefined function: "+name);
		return;
	}
	FunctionInfo info = m_functions[name];
	for(int i=0; i< functionCall->m_args->size(); i++){
		Expression* arg =functionCall->m_args->at(i); 
		check(arg);
		checkTypeEquality(token->m_line,info.m_args[i].m_type,getType(arg));

	}
}

int SematicAn::score(TokenType type){
	switch(type){
		case TokenType::STRING:
		case TokenType::IDEN_STRING:return 0;
		case TokenType::DOUBLE:
		case TokenType::IDEN_DOUBLE: return 1;
		case TokenType::IDEN_INT:
		case TokenType::INT:
		case TokenType::BOOL:
		case TokenType::IDEN_BOOL: return 2;
		case TokenType::IDEN_CHAR:
		case TokenType::CHAR: return 3;
	}
	return -1;
}

void SematicAn::checkTypeEquality(int line, TokenType one, TokenType two){
	if(two == TokenType::VOID){
		ErrorThrower::error(line,"Can not use void function as value");
	}
	if(one == TokenType::VAR || two == TokenType::VAR)return;
	if(one == TokenType::IDEN_STRING)return;
	int scoreSec = score(one);
	int scoreS = score(two);
	if(scoreS == -1 || scoreSec == -1) return;
	if(scoreSec > scoreS){
		ErrorThrower::error(line,"Can not convert data type please try casting");
	}
}

void SematicAn::checkDecleration(Decleration* decleration){
	
	Token* name = ((Literal*)decleration->m_name)->m_token;
	if(isReserved(name->m_symbol)){
		ErrorThrower::error(name->m_line,"Illigal use of reserved identifier");
	}
	if(decleration->m_initalize){
		Token* type = decleration->m_type->m_token;
		checkTypeEquality(type->m_line,type->m_type,getType(decleration->m_value));

		m_vars.push_back(Lock(m_level,name->m_symbol,TypeInfo(type->m_symbol,type->m_type,decleration->m_isArray)));

		if(type->m_type ==TokenType::IDEN){
			if(m_structs.count(type->m_symbol) ==0){
				ErrorThrower::error(type->m_line,"Unkown type");
			}
		}
	}else{
		check(decleration->m_name);
		if(decleration->m_name->name() == "Literal"){
			Token* token =((Literal*)decleration->m_name)->m_token; 
			checkTypeEquality(token->m_line,token->m_type,getType(decleration->m_value));
		}


	}
	check(decleration->m_value);
}

void SematicAn::checkArrayLiteral(ArrayLiteral* literal){}
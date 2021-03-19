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

TypeInfo SematicAn::getTypeBin(BinOP*  op){
	TokenType os = op->m_op->m_type;
	switch(os){
		case TokenType::TIMES_TIMES:
		case TokenType::DIVIDE_DIVIDE:return TypeInfo("bool",TokenType::DOUBLE,false);
		case TokenType::OR_OR:
		case TokenType::AND_AND:
		case TokenType::LESS:
		case TokenType::GREATER:
		case TokenType::MORE_EQUAL:
		case TokenType::LESS_EQUAL:return TypeInfo("bool",TokenType::BOOL,false);
	}
	TypeInfo left = getType(op->m_left);
	TypeInfo right = getType(op->m_right);
	if(left.m_type == TokenType::STRING || right.m_type == TokenType::STRING){
		return TypeInfo("string",TokenType::STRING,false);
	}else if(left.m_type == TokenType::DOUBLE || right.m_type == TokenType::DOUBLE){
		return TypeInfo("double",TokenType::DOUBLE,false);
	}else if(left.m_type == TokenType::INT || right.m_type == TokenType::INT){
		return TypeInfo("int",TokenType::INT,false);
	}else if(right.m_type == TokenType::CHAR ||left.m_type == TokenType::CHAR ){
		return TypeInfo("int",TokenType::INT,false);
	}else if(right.m_type == TokenType::BOOL || left.m_type == TokenType::BOOL){
		return TypeInfo("int",TokenType::INT,false);
	}
	return TypeInfo("var",TokenType::VAR,false);
}

TypeInfo SematicAn::getType(Expression* expression){
	if(expression == nullptr)
		return TypeInfo("var",TokenType::VAR,false);
	if(expression->name() == "BinOP"){
		return  getTypeBin((BinOP*)expression);
	}else if(expression->name() == "Cast"){
		Cast* cast = (Cast*)expression;
		return TypeInfo(typeToString(cast->m_iden->m_token->m_type),cast->m_iden->m_token->m_type,false);
	}else if(expression->name() == "Literal"){
		Literal* lit = (Literal*)expression;
		return TypeInfo(typeToString(lit->m_token->m_type),lit->m_token->m_type,false);

	}else if(expression->name() == "ArrayLiteral"){
		ArrayLiteral* literal = (ArrayLiteral*)expression;
		Token* name =literal->m_iden->m_token; 
		if(name->m_type== TokenType::IDEN){
			TypeInfo info = getVar(name->m_symbol).m_type;
			if(!info.m_isArray &&info.m_type == TokenType::IDEN_STRING){
				return TypeInfo("char",TokenType::IDEN_CHAR,false);
			}
			info.m_isArray = false;

			return info;  
		}else{
			if(name->m_type == TokenType::STRING){
				TypeInfo("char",TokenType::IDEN_CHAR,false);
			}
			return TypeInfo(typeToString(name->m_type),name->m_type,true );
		}
	}else if(expression->name() == "FunctionCall"){
		FunctionCall* call = (FunctionCall*)expression;
		string name = call->m_name->m_token->m_symbol +" with args count "+to_string(call->m_args->size());
		return m_functions[name].m_info;
	}else if(expression->name() == "Array"){
		return TypeInfo("var",TokenType::VAR,true);
	}
	return TypeInfo("var",TokenType::VAR,false);
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

bool SematicAn::checkReturns(Body* body,TypeInfo type){
	bool hasReturn = false;
	for(Expression* expr: *body->m_lines){
		if(expr->name() == "BuiltIn"){
			BuiltIn* name = ((BuiltIn*)expr);
			if(name->m_type->m_token->m_type == TokenType::RETURN){
				if(type.m_type != TokenType::VOID)
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
		bool hasRet = checkReturns(body,TypeInfo(typeToString(type),type,func->m_isArray));
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

int SematicAn::getLine(Expression* expression){
	if(expression == nullptr)
		return 0;
	if(expression->name() == "Literal"){
		return ((Literal*)expression)->m_token->m_line;
	}else if(expression->name() == "BinOP"){
		return getLine(((BinOP*)expression)->m_left);
	}else if(expression->name() == "Dot"){
		return getLine(((Dot*)expression)->m_iden);
	}else if(expression->name() == "FunctionCall"){
		return getLine(((FunctionCall*)expression)->m_name);
	}else if(expression->name() == "BuiltIn"){
		return getLine(((BuiltIn*)expression)->m_type);
	}else if(expression->name() == "Cast"){
		return getLine(((Cast*)expression)->m_iden);
	}else if(expression->name() == "ArrayLiteral"){
		return getLine(((ArrayLiteral*)expression)->m_iden);
	}
	return 0;
}

void SematicAn::controlStatements(Expression* expression){
	if(expression == nullptr)
		return;
	string name = expression->name();
	if(name == "For"){
		ForStat* stat = (ForStat*)expression;
		check(stat->m_initial);
		check(stat->m_condition);
		checkTypeEquality(getLine(stat->m_condition),TypeInfo("bool",TokenType::BOOL,false),getType(stat->m_condition));
		check(stat->m_repitition);
	}else if(name=="If"){
		IfStat* stat = (IfStat*)expression;
		check(stat->m_control);
		checkTypeEquality(getLine(stat->m_control),TypeInfo("bool",TokenType::BOOL,false),getType(stat->m_control));

	}else if(name == "While"){
		IfStat* stat = (IfStat*)expression;
		check(stat->m_control);
		checkTypeEquality(getLine(stat->m_control),TypeInfo("bool",TokenType::BOOL,false),getType(stat->m_control));
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
	m_functions[name] =  FunctionInfo(TypeInfo(token->m_symbol,token->m_type,expr->m_isArray),args);
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
		TokenType type =info.m_args[i].m_type; 
		checkTypeEquality(token->m_line,TypeInfo(typeToString(type),type,info.m_info.m_isArray),getType(arg));

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

void SematicAn::checkTypeEquality(int line, TypeInfo oneT, TypeInfo twoT){
	TokenType one = oneT.m_type;
	TokenType two = twoT.m_type;
	if(oneT.m_isArray != twoT.m_isArray){
		ErrorThrower::error(line,"can not convert array into no array value");
	}
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
	TypeInfo info;
	if(decleration->m_initalize){
		Token* type = decleration->m_type->m_token;
		 info =TypeInfo(typeToString(type->m_type),type->m_type,decleration->m_isArray); 
		checkTypeEquality(type->m_line,info,getType(decleration->m_value));

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
			checkTypeEquality(token->m_line,TypeInfo(typeToString(token->m_type),token->m_type,decleration->m_isArray),getType(decleration->m_value));
		}else if(decleration->m_name->name() == "ArrayLiteral"){
			ArrayLiteral* literal = (ArrayLiteral*)decleration->m_name;
			info = getType(literal);
			checkTypeEquality(getLine(literal),info,getType(decleration->m_value));
		}


	}
	if(decleration->m_value != nullptr&& decleration->m_value->name() == "Array"){
			Array* array = (Array*)decleration->m_value; 
			info.m_isArray = false;
			for(Expression* arg: *array->m_args){
				checkTypeEquality(getLine(decleration->m_name),info,getType(arg));
			}

	}
	check(decleration->m_value);
}

void SematicAn::checkArrayLiteral(ArrayLiteral* literal){
	TokenType type =literal->m_iden->m_token->m_type ; 
	if(type == TokenType::IDEN || type == TokenType::STRING){
		check(literal->m_iden);
		checkTypeEquality(getLine(literal->m_iden),TypeInfo("int",TokenType::INT,false),getType(literal->m_value));
	}else{
		if(literal->m_value != nullptr){
			ErrorThrower::error(getLine(literal->m_value),"can not use number when declaring array ex: int[] ");
		}
	}
}
//The purpose of this file is to symatically analize a AST

#include "SematicAn.h"
#include "ErrorThrower.h"


SematicAn::SematicAn(Body* body){
	m_body  = body;
	m_level =0;
	string argsMes = " with args count ";
	vector<TypeInfo> info  ={TypeInfo("List",TokenType::VAR,true), TypeInfo("int",TokenType::INT,false)};
	m_functions.push_back(FunctionInfo(0,"delete"+argsMes+"2", TypeInfo("void",TokenType::VOID,false),info ));
	info  ={};
	m_functions.push_back(FunctionInfo(0,"input"+argsMes+"0" , TypeInfo("string",TokenType::STRING,false),info ));
	info  ={TypeInfo("var",TokenType::VAR,true), TypeInfo("var",TokenType::VAR,false)};
	m_functions.push_back(FunctionInfo(0,"append"+argsMes+"2" , TypeInfo("void",TokenType::VOID,false),info ));
	info  ={TypeInfo("string",TokenType::STRING,false)};
	m_functions.push_back(FunctionInfo(0,"deleteFile"+argsMes+"1" , TypeInfo("void",TokenType::VOID,false),info ));
	m_functions.push_back(FunctionInfo(0,"createFile"+argsMes+"1" , TypeInfo("void",TokenType::VOID,false),info ));
	m_functions.push_back(FunctionInfo(0,"exists"+argsMes+"1" , TypeInfo("int",TokenType::INT,false),info ));
	info  ={TypeInfo("string",TokenType::STRING,false),TypeInfo("string",TokenType::STRING,false)};
	m_functions.push_back(FunctionInfo(0,"writeFile"+argsMes+"2" , TypeInfo("void",TokenType::VOID,false),info ));
	info  ={TypeInfo("string",TokenType::STRING,false)};
	m_functions.push_back(FunctionInfo(0,"readFile"+argsMes+"1" , TypeInfo("string",TokenType::LIST,true),info ));
	info  ={TypeInfo("double",TokenType::DOUBLE,false)};
	m_functions.push_back(FunctionInfo(0,"sin"+argsMes+"1" , TypeInfo("double",TokenType::DOUBLE,true),info ));
	m_functions.push_back(FunctionInfo(0,"cos"+argsMes+"1" , TypeInfo("double",TokenType::DOUBLE,true),info ));
	m_functions.push_back(FunctionInfo(0,"tan"+argsMes+"1" , TypeInfo("double",TokenType::DOUBLE,true),info ));
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

TypeInfo SematicAn::getTypeDot(Dot* dot){
	TypeInfo info = getType(dot->m_iden);
	if(dot->m_subIden->name() == "FunctionCall"){
		FunctionCall* call = (FunctionCall*)dot->m_subIden;
		string name = call->m_name->m_token->m_symbol+ " with args count "+to_string(call->m_args->size());
		return getFunc(m_structsFunctions[info.m_symbol],name).m_info; 
	}else if(dot->m_subIden->name() == "Literal"){
		Literal* literal = (Literal*)dot->m_subIden;
		return m_structs[info.m_symbol][literal->m_token->m_symbol];
	}
	return TypeInfo("var",TokenType::VAR,false);

}

TypeInfo SematicAn::getType(Expression* expression){
	if(expression == nullptr)
		return TypeInfo("var",TokenType::VAR,false);
	if(expression->name() == "BinOP"){
		return  getTypeBin((BinOP*)expression);
	}else if(expression->name() == "Dot"){
		return getTypeDot((Dot*)expression);
	}
	else if(expression->name() == "BuiltIn"){
		BuiltIn* builtin = (BuiltIn*)expression;
		if(builtin->m_type->m_token->m_type == TokenType::LEN){
			return TypeInfo("int",TokenType::INT,false);
		}else if(builtin->m_type->m_token->m_type == TokenType::NEW){
			return TypeInfo("var",TokenType::VAR,false);
		}
		return TypeInfo("void",TokenType::VOID,false);
	}else if(expression->name() == "Cast"){
		Cast* cast = (Cast*)expression;
		return TypeInfo(typeToString(cast->m_iden->m_token->m_type),cast->m_iden->m_token->m_type,false);
	}else if(expression->name() == "Literal"){
		Literal* lit = (Literal*)expression;
		if(lit->m_token->m_symbol == "this"){
			return TypeInfo(m_inside,TokenType::STRUCT,false);
		}
		if(m_structs.count(lit->m_token->m_symbol) !=0){
			return TypeInfo(lit->m_token->m_symbol,TokenType::NIL,false);
		}
		if(lit->m_token->m_type == TokenType::IDEN){
			return getVar(lit->m_token->m_symbol).m_type;
		}
		return TypeInfo(typeToString(lit->m_token->m_type),lit->m_token->m_type,false);

	}else if(expression->name() == "ArrayLiteral"){
		ArrayLiteral* literal = (ArrayLiteral*)expression;
		Token* name =literal->m_iden->m_token; 
		if(name->m_type== TokenType::IDEN&& (m_structs.count(name->m_symbol) == 0)){
			TypeInfo info = getVar(name->m_symbol).m_type;
			if(info.m_map){
				info.m_map = false;
				info.m_type = info.m_mapType;
			}
			if(!info.m_isArray &&info.m_type == TokenType::IDEN_STRING){
				return TypeInfo("char",TokenType::IDEN_CHAR,false);
			}
			info.m_isArray = false;

			return info;  
		}else{
			if(name->m_type == TokenType::STRING){
				TypeInfo("char",TokenType::IDEN_CHAR,false);
			}
			if(literal->m_value != nullptr && literal->m_value->name() == "Literal"){
				Literal* type= (Literal*)(literal->m_value);
				return TypeInfo(typeToString(name->m_type),name->m_type,true,true, type->m_token->m_type);

			}
			return TypeInfo(typeToString(name->m_type),name->m_type,true);
		}
	}else if(expression->name() == "FunctionCall"){
		FunctionCall* call = (FunctionCall*)expression;
		string name = call->m_name->m_token->m_symbol +" with args count "+to_string(call->m_args->size());
		return getFunc(m_functions,name).m_info;
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
	}else if(name == "ForArray"){
		ForArray* array = (ForArray*)expr;
		check(array->m_dect);
		check(array->m_value);
	}
	else if(name == "FunctionCall"){
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
	while(m_functions.size() >0 && m_functions[m_functions.size()-1].m_level == m_level){
		m_functions.pop_back();
	}
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
	TypeInfo info = getType(dot->m_iden);
	if(info.m_type != TokenType::NIL)
		check(dot->m_iden);		


	if(dot->m_subIden->name() == "FunctionCall"){
		FunctionCall* call = (FunctionCall*)dot->m_subIden;
		string name =call->m_name->m_token->m_symbol+" with args count "+to_string(call->m_args->size()); 
		if(info.m_map && (name =="keys with args count 0" ||name == "vals with args count 0"))
			return;
		if(!containsFunc(m_structsFunctions[info.m_symbol],name)){
			ErrorThrower::error(call->m_name->m_token->m_line,"Undefined class call");
			return;
		}	
		FunctionInfo funcInfo = getFunc(m_structsFunctions[info.m_symbol],name);
		if(info.m_type == TokenType::NIL &&!funcInfo.m_static){
			ErrorThrower::error(call->m_name->m_token->m_line, "Static call of non ststic function");
		}
		for(int i=0;  i < funcInfo.m_args.size(); i++){
			checkTypeEquality(call->m_name->m_token->m_line,funcInfo.m_args[i],getType(call->m_args->at(i)));

		}

	}else if(dot->m_subIden->name() == "Literal"){
		Literal* literal = (Literal*)dot->m_subIden;
		string name =info.m_symbol; 
		if(m_structs[name].count(literal->m_token->m_symbol ) ==0){
			ErrorThrower::error(literal->m_token->m_line,"Undefined class variable");	
			return;
		}
	}
	
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
	}else 
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
	vector<FunctionInfo> funcInfo;
	if(control->m_extends){
		if(m_structs.count(control->m_extender->m_token->m_symbol) != 0){
			vector<FunctionInfo> info = m_structsFunctions[control->m_extender->m_token->m_symbol];
			for(FunctionInfo in: info){
				in.m_level = m_level;

				funcInfo.push_back(in);		
				m_functions.push_back(in);
			}
		}else
			ErrorThrower::error(getLine(control->m_extender),"extends unknown class");
	}
	m_structsFunctions[name] = funcInfo;

	m_inside= name;
	for(int i=0; i< body->m_lines->size(); i++){
		Expression* expr =body->m_lines->at(i); 
		if(expr->name() == "Decleration"){
		
			Decleration* dect = (Decleration*)expr;
		
			string nameVar = ((Literal*)dect->m_name)->m_token->m_symbol;
			Token* type =dect->m_type->m_token;
			TypeInfo info =TypeInfo(type->m_symbol,type->m_type, dect->m_isArray); 
			m_structs[name][nameVar] = info; 
			m_vars.push_back(Lock(m_level,nameVar,info));
		}else if(expr->name() == "Body"){
			increaseLevel();
			Body* body = (Body*)expr;
			if(body->m_control->name() == "Function"){
				Function* func = (Function*)body->m_control; 	
				FunctionCall* call = func->m_call;	
				vector<TypeInfo> argInfo;
				for(Expression* arg:*call->m_args){
					if(arg->name() != "Decleration"){
						argInfo.push_back(TypeInfo("",TokenType::NIL,false));
						ErrorThrower::error(getLine(arg),"illigal use of non decleration as argument");
						continue;
					}
					Decleration* decleration = (Decleration*)arg;
					if(!decleration->m_initalize){
						argInfo.push_back(TypeInfo("",TokenType::NIL,false));
						ErrorThrower::error(getLine(arg),"illigal use of non decleration as argument");
						continue;
					}
					Token* token = decleration->m_type->m_token;
					TypeInfo info  =TypeInfo(token->m_symbol,token->m_type,decleration->m_isArray); 
					m_vars.push_back(Lock(m_level,((Literal*)decleration->m_name)->m_token->m_symbol,info));
					argInfo.push_back(info);	
				}
				string nameFunc = call->m_name->m_token->m_symbol;
				Token* token = func->m_type->m_token;
				FunctionInfo info =FunctionInfo(func->m_isStatic,m_level,nameFunc+" with args count "+to_string(call->m_args->size()) ,TypeInfo(token->m_symbol,token->m_type,func->m_isArray),argInfo); 
				m_functions.push_back(info);
				m_structsFunctions[name].push_back(info);
				bool hasRet = checkReturns(body,TypeInfo(token->m_symbol,token->m_type,func->m_isArray));
				if(!hasRet && token->m_type!= TokenType::VOID)
					ErrorThrower::error(func->m_type->m_token->m_line,"Function can reach end without return");


			}

			for(Expression* exp: *body->m_lines){
				check(exp);
			}
			
			popLevel();
		}

		
	}
	m_inside = "";
	

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
		if(stat->m_initial == nullptr ||stat->m_initial->name()!="ForArray" ){

			check(stat->m_condition);
			checkTypeEquality(getLine(stat->m_condition),TypeInfo("bool",TokenType::BOOL,false),getType(stat->m_condition));
			check(stat->m_repitition);
		}
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
bool SematicAn::containsFunc(vector<FunctionInfo> funcs,string name ){
	for(FunctionInfo info : funcs){
		if(name == info.m_name)
			return true;
	}
	return false;
}

FunctionInfo SematicAn::getFunc(vector<FunctionInfo> funcs,string name ){
	for(FunctionInfo info : funcs){
		if(name == info.m_name)
			return info;
	}
	return FunctionInfo();
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
	m_functions.push_back(FunctionInfo(m_level,name,TypeInfo(token->m_symbol,token->m_type,expr->m_isArray),args));
	for(Expression* arg: *call->m_args){
		check(arg);
	}
		
	

}


void SematicAn::checkFunctionCall(FunctionCall* functionCall){
	Token* token = functionCall->m_name->m_token; 
	string name =token->m_symbol+" with args count "+to_string(functionCall->m_args->size());  
	
	if(containsFunc(m_functions,name) ==0 && m_inside == "" ){
		ErrorThrower::error(token->m_line, "Undefined function: "+name);
		return;
	}
	if(name == "append with args count 2"){
		TypeInfo type = getType(functionCall->m_args->at(0));
		type.m_isArray = false;
		checkTypeEquality(getLine(functionCall),type,getType(functionCall->m_args->at(1)));
		return;
	}

	FunctionInfo info = getFunc(m_functions,name);
	for(int i=0; i< functionCall->m_args->size(); i++){
		Expression* arg =functionCall->m_args->at(i); 
		check(arg);
		TypeInfo type =info.m_args[i]; 
		if(arg->name() != "Array")
			checkTypeEquality(token->m_line,type,getType(arg));
		else {
			Array* arr = (Array*)arg;
			type.m_isArray = false;
			for(int j=0; j< arr->m_args->size(); j++){
				checkTypeEquality(token->m_line,type,getType(arr->m_args->at(j)));
			}
		}

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
		ErrorThrower::error(line,"can not convert array into non array value");
	}
	if(oneT.m_map != twoT.m_map){
		ErrorThrower::error(line,"can not convert map into non map value");
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
		TokenType mapType;
		if(decleration->m_isMap){
			mapType = decleration->m_mapType->m_token->m_type;
		}
		 info =TypeInfo(typeToString(type->m_type),type->m_type,decleration->m_isArray,decleration->m_isMap,mapType); 
		 if(decleration->m_value != nullptr)
			checkTypeEquality(type->m_line,info,getType(decleration->m_value));

		 TypeInfo inf(type->m_symbol,type->m_type,decleration->m_isArray,decleration->m_isMap,mapType); 
		m_vars.push_back(Lock(m_level,name->m_symbol,inf));
		

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
	if(m_structs.count(literal->m_iden->m_token->m_symbol) ==0 && type == TokenType::IDEN || type == TokenType::STRING){
		check(literal->m_iden);
	}else{
		if(literal->m_value != nullptr ){
			TypeInfo type = getType(literal->m_value);
			if(type.m_type == TokenType::INT)
				ErrorThrower::error(getLine(literal->m_value),"can not use number when declaring array ex: int[] ");
		}
	}
}

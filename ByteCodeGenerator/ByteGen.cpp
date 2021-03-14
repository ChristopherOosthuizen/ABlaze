
#include "ByteGen.h"
#include <algorithm>

ByteGen::ByteGen(Body* ast){
        m_ast = ast;
        m_lines = new vector<string>(); 
}

vector<string>* ByteGen::generateByteCode(){
        for(int i=0; i< m_ast->m_lines->size(); i++){
                if(m_ast->m_lines->at(i)->name() == "Body" && ((Body*)m_ast->m_lines->at(i))->m_control->name() == "Struct"){

                        structDec((Body*)(m_ast->m_lines->at(i)));
                }else if(m_ast->m_lines->at(i)->name() == "Decleration"){
                       decToCommand((Decleration*)(m_ast->m_lines->at(i)) ); 
                }
        }
        toCommand("call");
        toCommand("main");
        bodyToByte(m_ast);
        return m_lines;
}
void ByteGen::toCommand(const string& com){
        m_lines->push_back(com);
}

void ByteGen::expressionToByte(Expression* expr){
        string name = expr->name(); 
        if(name == "Decleration"){
                decToCommand((Decleration*)expr);
       }else if(name == "BinOP"){
                ByteGen::binToCommand((BinOP*)expr);
        }else if(name =="FunctionCall"){
                functionCallToByte((FunctionCall*)expr);
        }else if(name =="Literal"){
                TokenType type = ((Literal*)expr)->m_token->m_type;
                if(type == TokenType::IDEN){
                        toCommand("load");
                        toCommand(((Literal*)expr)->m_token->m_symbol); 
                }
                else {
                        toCommand("push");
                        if(((Literal*)expr)->m_token->m_type == TokenType::STRING)
                                toCommand("\""+((Literal*)expr)->m_token->m_symbol+"\"");
                        else
                                toCommand(((Literal*)expr)->m_token->m_symbol);
                }
        }else if(name == "Body"){
                bodyToByte((Body*)expr);
        }else if(name == "UnOP"){
                unToByte((Unary*)expr);
        }else if(name == "ArrayLiteral"){
                arrayToByte((ArrayLiteral*) expr);
        }else if(name == "Dot"){
                dotToByte((Dot*) expr);
        }else if(name == "BuiltIn"){
                builtInToByte((BuiltIn*) expr);
        }else if(name == "Cast"){
                castToByte((Cast*)expr);
        }

}

void ByteGen::builtInToByte(BuiltIn* builtin){
        TokenType name = builtin->m_type->m_token->m_type;
        if(name ==TokenType::NEW){
                if(builtin->m_value->name() == "Literal"){
                        Literal* value = (Literal*)builtin->m_value;
                        toCommand(typeToString(name));

                        toCommand(value->m_token->m_symbol);
                }else{
                        FunctionCall* call = (FunctionCall*)builtin->m_value;
                        for(Expression* expr:*call->m_args){
                                expressionToByte(expr);
                        }
                        toCommand(typeToString(name));
                        toCommand(call->m_name->m_token->m_symbol);

                }
                return;
        }

        expressionToByte(builtin->m_value);
        toCommand(typeToString(name));
}

void ByteGen::argsToBye(vector<Expression*>* args){
        for(Expression* expr:*args){
                expressionToByte(expr);
        }
}

void ByteGen::dotToByte(Dot* dot){
        expressionToByte(dot->m_iden);
        if(dot->m_subIden->name() == "Literal"){
                toCommand("select");
                toCommand(((Literal*)dot->m_subIden)->m_token->m_symbol);
        }else if(dot->m_subIden->name() == "FunctionCall"){
                FunctionCall* call = (FunctionCall*)dot->m_subIden;
                argsToBye(call->m_args);
                
                toCommand("classcall");
                toCommand(call->m_name->m_token->m_symbol);
        }
}

void ByteGen::arrayToByte(ArrayLiteral* literal){
        if(literal->m_value == nullptr){
                toCommand("new");
                toCommand("list");
                return;
        }
        expressionToByte(literal->m_iden);
        expressionToByte(literal->m_value);
        toCommand("at");
}

void ByteGen::bodyToByte(Body* body){
        for(int i=0; i< body->m_lines->size();i++){
                Expression* current = body->m_lines->at(i);
                if(current->name() == "Body"){
                        Body* bod = (Body*)current;
                        string line = to_string(m_lines->size());
                        if(bod->m_control->name() == "If"){
                                toCommand("startlocal");
                                expressionToByte(((IfStat*)bod->m_control)->m_control);
                                toCommand("jif");
                                toCommand("startif"+line);
                                if(body->m_lines->size()-1 >i && body->m_lines->at(i+1)->name() == "Body" && ((Body*)body->m_lines->at(i+1))->m_control->name() == "Else"){
                                        bodyToByte((Body*)body->m_lines->at(i+1));
                                        i++;
                                }
                                toCommand("jmp");
                                toCommand("endif"+line);
                                toCommand("startif"+line+":");
                                bodyToByte(bod);
                                toCommand("endif"+line+":");
                                toCommand("poplocal");
                        }else if(bod->m_control->name() == "While"){
                                whileToByte(bod,line);
                        }else if(bod->m_control->name() =="For"){
                                forToByte(bod,line);
                        }else if(bod->m_control->name() == "Function"){
                                functionToByte(bod);
                        }else if(bod->m_control->name() == "Struct"){
                                structToByte(bod);
                        }
                }
                else 
                        expressionToByte(current);
                
               
        }

}

void ByteGen::classFunc(string name, Body* body){
        Function* function = (Function*)body->m_control;
        FunctionCall* call = (FunctionCall*)function->m_call;
        string subname =call->m_name->m_token->m_symbol; 
        string address;
        if(name != subname)
                address = name +"."+subname+":";
        else 
                address = name+":";
       toCommand(address);
       toCommand("loadclass");
       toCommand("store");
       toCommand("this");
        for(int i=call->m_args->size()-1; i>=0 ;i--){
                expressionToByte(call->m_args->at(i));
        }
       bodyToByte(body);
       if(name == subname){
        toCommand("load");
        toCommand("this");
       }
       toCommand("return");
}

void ByteGen::structToByte(Body* body){
        string name = ((Struct*)body->m_control)->m_iden->m_token->m_symbol;
         for(Expression* expression: *body->m_lines){
                 if(expression->name() == "Body")
                        classFunc(name,(Body*)expression);
         }


}


void ByteGen::structDec(Body* body){
        toCommand("startlocal");
         for(Expression* expression: *body->m_lines){
                 if(expression->name() == "Decleration")
                        decToCommand((Decleration*)expression);
                else if(expression->name() == "Body" && ((Body*)expression)->m_control->name() =="Function"){
                        toCommand("functionPush");
                        FunctionCall* call = ((Function*)((Body*)expression)->m_control)->m_call;
                        toCommand(call->m_name->m_token->m_symbol);
                }
         }
         Struct* control = (Struct*)body->m_control;
         bool extends = control->m_extends;
        if(!extends){
                toCommand("structdec");
                toCommand(((Struct*)body->m_control)->m_iden->m_token->m_symbol);
        }else{
                toCommand("structdecEx");
                toCommand(((Struct*)body->m_control)->m_iden->m_token->m_symbol);
                toCommand(control->m_extender->m_token->m_symbol);
        }

}


void ByteGen::unToByte(Unary* unary){
        TokenType type = unary->m_op->m_token->m_type;
        expressionToByte(unary->m_iden);
        toCommand("push");
        toCommand("1");
        switch(type){
                case TokenType::PLUS_PLUS: toCommand("add");break;
                case TokenType::MINUS_MINUS: toCommand("minus");break;

        }
        toCommand("asi");
        toCommand(((Literal*)unary->m_iden)->m_token->m_symbol);

}

void ByteGen::whileToByte(Body* body,string line){
        toCommand("startWhile"+line+":");
        toCommand("startlocal");
        expressionToByte(((WhileStat*)body->m_control)->m_control);
        toCommand("not");
        toCommand("startlocal");
        toCommand("jif");
        toCommand("endWhile"+line);
        bodyToByte(body);
        toCommand("poplocal");
        toCommand("jmp");
        toCommand("startWhile"+line);
        toCommand("endWhile"+line+":");
        toCommand("poplocal");

}

void ByteGen::forToByte(Body* body,string line){
        ForStat* stat =((ForStat*)body->m_control);
        toCommand("startlocal");
        expressionToByte(stat->m_initial);
        toCommand("startFor"+line+":");
        expressionToByte(stat->m_condition);
        toCommand("not");
        toCommand("startlocal");
        toCommand("jif");
        toCommand("endFor"+line);
        bodyToByte(body);
        expressionToByte(stat->m_repitition);
        toCommand("poplocal");
        toCommand("jmp");
        toCommand("startFor"+line);
        toCommand("endFor"+line+":");
        toCommand("poplocal");
}


void ByteGen::decToCommand(Decleration* dec){
        if(dec->m_value != nullptr)
                expressionToByte(dec->m_value);

        
        if(dec->m_name->name() =="Dot"){
                Dot* dot = (Dot*)dec->m_name;
                expressionToByte(dot->m_iden);
                toCommand("set");
                toCommand(((Literal*)dot->m_subIden)->m_token->m_symbol);
                return;
        }
        string name =((Literal*)dec->m_name)->m_token->m_symbol; 
        if(dec->m_initalize)
                toCommand("store");
        else{
                TokenType type = dec->m_op->m_token->m_type;
                if(type != TokenType::EQUAL){
                        toCommand("load");
                        toCommand(name);
                        switch(type){
                                case TokenType::PLUS_EQUAL:toCommand("plus");break; 
                                case TokenType::MINUS_EQUAL:toCommand("minus");break; 
                                case TokenType::DIVIDE_EQUAL:toCommand("divide");break; 
                                case TokenType::TIMES_EQUAL:toCommand("times");break; 
                                case TokenType::MOD_EQUAL:toCommand("mod");break; 
                        }

                }
                toCommand("asi");
        }
        toCommand(name); 

}

void ByteGen::binToCommand(BinOP* op){
                expressionToByte(op->m_left);
                expressionToByte(op->m_right);
                toCommand(typeToString(op->m_op->m_type));
}

void ByteGen::functionToByte(Body* body){

        FunctionCall* control = (FunctionCall*)((Function*)body->m_control)->m_call;
        string name = control->m_name->m_token->m_symbol; 
        toCommand(name+":");  
        for(int i=control->m_args->size()-1; i>=0 ;i--){
                expressionToByte(control->m_args->at(i));
        }
        bodyToByte(body);
        if(name != "main"){
                toCommand("return");
        }else
                toCommand("halt");
}

void ByteGen::castToByte(Cast* cast){
        expressionToByte(cast->m_value);
        toCommand("cast");
        toCommand(cast->m_iden->m_token->m_symbol);
}

bool ByteGen::isBuiltIn(string name){
        vector<string> funcs = {"delete","append","input","readFile","writeFile","exists","deleteFile","createFile","sin","cos","tan"};
        if(find(funcs.begin(),funcs.end(),name) != funcs.end())
                return 1;
        return 0;

}

void ByteGen::functionCallToByte(FunctionCall* call){
        string symb = call->m_name->m_token->m_symbol;
        argsToBye(call->m_args); 
        if(isBuiltIn(symb)){
                toCommand(symb);
                return;
        }
        
        toCommand("call");
        toCommand(symb);
}


#include "ByteGen.h"

ByteGen::ByteGen(Body* ast){
        m_ast = ast;
        m_lines = new vector<string>(); 
}

vector<string>* ByteGen::generateByteCode(){
        for(int i=0; i< m_ast->m_lines->size(); i++){
                if(m_ast->m_lines->at(i)->name() == "Body" && ((Body*)m_ast->m_lines->at(i))->m_control->name() == "Struct"){
                        structToByte((Body*)(m_ast->m_lines->at(i)));
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
                toCommand(Lexer::typeToString(name));
                Literal* value = (Literal*)builtin->m_value;
                toCommand(value->m_token->m_symbol);
                return;
        }

        expressionToByte(builtin->m_value);
        toCommand(Lexer::typeToString(name));
}


void ByteGen::dotToByte(Dot* dot){
        toCommand("load");
        toCommand(dot->m_iden->m_token->m_symbol);
        toCommand("select");
        toCommand(dot->m_subIden->m_token->m_symbol);
}

void ByteGen::arrayToByte(ArrayLiteral* literal){
        if(literal->m_value == nullptr){
                toCommand("new");
                toCommand("Array");
                return;
        }
        toCommand("load");
        toCommand(literal->m_iden->m_token->m_symbol);
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
                                continue;
                        }
                }
                else 
                        expressionToByte(current);
                
               
        }

}

void ByteGen::structToByte(Body* body){
         toCommand("startlocal");
        bodyToByte(body);
        toCommand("structdec");
        toCommand(((Struct*)body->m_control)->m_iden->m_token->m_symbol);
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
        string nameer=((Literal*)dec->m_name)->m_token->m_symbol;  
        if(dec->m_value != nullptr)
                expressionToByte(dec->m_value);

        
        if(dec->m_name->name() =="Dot"){
                Dot* dot = (Dot*)dec->m_name;
                toCommand("load");
                toCommand(dot->m_iden->m_token->m_symbol);
                toCommand("set");
                toCommand(dot->m_subIden->m_token->m_symbol);
                return;
        }

        if(dec->m_initalize)
                toCommand("store");
        else
                toCommand("asi");
        toCommand(((Literal*)dec->m_name)->m_token->m_symbol); 

}

void ByteGen::binToCommand(BinOP* op){
                expressionToByte(op->m_left);
                expressionToByte(op->m_right);
                toCommand(Lexer::typeToString(op->m_op->m_type));
}

void ByteGen::functionToByte(Body* body){

        FunctionCall* control = (FunctionCall*)((Function*)body->m_control)->m_call;
        string name = control->m_name->m_token->m_symbol; 
        toCommand(name+":");  
        for(int i=0; i< control->m_args->size();i++){
                toCommand("store");
                Decleration* dec =(Decleration*)control->m_args->at(i) ;
                toCommand(((Literal*)dec->m_name)->m_token->m_symbol);
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

void ByteGen::functionCallToByte(FunctionCall* call){
        string symb = call->m_name->m_token->m_symbol;
        for(int i=0; i< call->m_args->size();i++){
                expressionToByte(call->m_args->at(i));
        }
        if(symb == "delete" || symb =="append" || symb == "input"){
                toCommand(symb);
                return;
        }
        
        toCommand("call");
        toCommand(symb);
}

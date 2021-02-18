
#include "ByteGen.h"

ByteGen::ByteGen(Body* ast){
        m_ast = ast;
        m_lines = new vector<string>(); 
}

vector<string>* ByteGen::generateByteCode(){
        bodyToByte(m_ast);
        toCommand("halt");
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
                        toCommand(((Literal*)expr)->m_token->m_symbol);
                }
        }else if(name == "Body"){
                bodyToByte((Body*)expr);
        }

}

void ByteGen::bodyToByte(Body* body){
        for(int i=0; i< body->m_lines->size();i++){
                Expression* current = body->m_lines->at(i);
                if(current->name() == "Body"){
                        Body* bod = (Body*)current;
                        string line = to_string(m_lines->size());
                        if(bod->m_control->name() == "If"){
                                expressionToByte(((IfStat*)bod->m_control)->m_control);
                                toCommand("jif");
                                toCommand("startif"+line);
                                if(body->m_lines->at(i+1)->name() == "Body" && ((Body*)body->m_lines->at(i+1))->m_control->name() == "Else"){
                                        bodyToByte((Body*)body->m_lines->at(i+1));
                                        i++;
                                }
                                toCommand("jmp");
                                toCommand("endif"+line);
                                toCommand("startif"+line+":");
                                bodyToByte(bod);
                                toCommand("endif"+line+":");
                        }else if(bod->m_control->name() == "While"){
                                whileToByte(bod,line);
                        }else if(bod->m_control->name() =="For"){
                                forToByte(bod,line);
                        }
                }
                else 
                        expressionToByte(current);
                
               
        }

}

void ByteGen::whileToByte(Body* body,string line){
        toCommand("startWhile"+line+":");
        expressionToByte(((WhileStat*)body->m_control)->m_control);
        toCommand("not");
        toCommand("jif");
        toCommand("endWhile"+line);
        bodyToByte(body);
        toCommand("jmp");
        toCommand("startWhile"+line);
        toCommand("endWhile"+line+":");
}

void ByteGen::forToByte(Body* body,string line){
        ForStat* stat =((ForStat*)body->m_control);
        expressionToByte(stat->m_initial);
        toCommand("startFor"+line+":");
        expressionToByte(stat->m_condition);
        toCommand("not");
        toCommand("jif");
        toCommand("endFor"+line);
        bodyToByte(body);
        expressionToByte(stat->m_repitition);
        toCommand("jmp");
        toCommand("startFor"+line);
        toCommand("endFor"+line+":");
}


void ByteGen::decToCommand(Decleration* dec){
        string nameer=((Literal*)dec->m_name)->m_token->m_symbol;  
        expressionToByte(dec->m_value);
        toCommand("store");
        toCommand(((Literal*)dec->m_name)->m_token->m_symbol); 

}

void ByteGen::binToCommand(BinOP* op){
                expressionToByte(op->m_left);
                expressionToByte(op->m_right);
                toCommand(Lexer::typeToString(op->m_op->m_type));
}

void ByteGen::functionCallToByte(FunctionCall* call){
        string symb = call->m_name->m_token->m_symbol;
        for(int i=0; i< call->m_args->size();i++){
                expressionToByte(call->m_args->at(i));
        }
        m_lines->push_back("print");
}

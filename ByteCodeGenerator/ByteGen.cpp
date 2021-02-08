#include "ByteGen.h"

ByteGen::ByteGen(Body* ast){
        m_ast = ast;
        m_lines = new vector<string>(); 
}

vector<string>* ByteGen::generateByteCode(){
        expressionToByte(m_ast->m_lines->at(0));
        return m_lines;
}

string ByteGen::toCommand(const string& com, const string& param1, const string& param2){
        return com +" "+param1+" , "+param2;
}

void ByteGen::expressionToByte(Expression* expr){
        string name = expr->name(); 
        if(name == "Decleration"){
                Decleration* dec = (Decleration*)expr;
                string nameer=((Literal*)dec->m_name)->m_token->m_symbol;  
                if(dec->m_initalize)
                        m_lines->push_back(toCommand("create",nameer,Lexer::typeToString(dec->m_type->m_token->m_type) ));
                expressionToByte(dec->m_value);
                m_lines->push_back(toCommand("assign","["+((Literal*)dec->m_name)->m_token->m_symbol+"]","memp"));
        }else if(name == "BinOP"){
                BinOP* op = (BinOP*)expr;
                if(op->m_left->name() == "Literal" && op->m_right->name() == "Literal"){
                        m_lines->push_back(toCommand("asi","memp",((Literal*)op->m_left)->m_token->m_symbol));
                        m_lines->push_back(toCommand(Lexer::typeToString(op->m_op->m_type),"memp",((Literal*)op->m_right)->m_token->m_symbol));
                }else if(op->m_left->name() == "Literal"){
                        expressionToByte(op->m_right);
                        m_lines->push_back(toCommand(Lexer::typeToString(op->m_op->m_type),"memp",((Literal*)op->m_left)->m_token->m_symbol));

                }
        }

                
}



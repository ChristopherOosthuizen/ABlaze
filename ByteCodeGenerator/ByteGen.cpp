#include "ByteGen.h"

ByteGen::ByteGen(Body* ast){
        m_ast = ast;
        m_lines = new vector<string>(); 
}

vector<string>* ByteGen::generateByteCode(){
        expressionToByte(m_ast->m_lines->at(0));
        return m_lines;
}

void ByteGen::toCommand(const string& com, const string& param1, const string& param2){
        string str = com +" "+param1+" , "+param2; 
        m_lines->push_back(str);
}

void ByteGen::expressionToByte(Expression* expr){
        string name = expr->name(); 
        if(name == "Decleration"){
                decToCommand((Decleration*)expr);
       }else if(name == "BinOP"){
                ByteGen::binToCommand((BinOP*)expr);
        }

                
}

void ByteGen::decToCommand(Decleration* dec){
        string nameer=((Literal*)dec->m_name)->m_token->m_symbol;  
        if(dec->m_initalize)
                toCommand("create",nameer,Lexer::typeToString(dec->m_type->m_token->m_type) );
        expressionToByte(dec->m_value);
        toCommand("assign","["+((Literal*)dec->m_name)->m_token->m_symbol+"]","memp");

}

void ByteGen::binToCommand(BinOP* op){
        if(op->m_left->name() == "Literal" && op->m_right->name() == "Literal"){
                toCommand("asi","memp",((Literal*)op->m_left)->m_token->m_symbol);
                toCommand(Lexer::typeToString(op->m_op->m_type),"memp",((Literal*)op->m_right)->m_token->m_symbol);
        }else if(op->m_left->name() == "Literal" && op->m_right->name() !="Literal"){
                expressionToByte(op->m_right);
                toCommand(Lexer::typeToString(op->m_op->m_type),"memp",((Literal*)op->m_left)->m_token->m_symbol);
        }else if(op->m_left->name() != "Literal" && op->m_right->name() =="Literal"){
                expressionToByte(op->m_left);
                toCommand(Lexer::typeToString(op->m_op->m_type),"memp",((Literal*)op->m_right)->m_token->m_symbol);
        }else {
                expressionToByte(op->m_left);
                expressionToByte(op->m_right);
                toCommand(Lexer::typeToString(op->m_op->m_type),"memp","memp");

        }
}

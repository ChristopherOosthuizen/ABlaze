
#include "ByteGen.h"

ByteGen::ByteGen(Body* ast){
        m_ast = ast;
        m_lines = new vector<string>(); 
}

vector<string>* ByteGen::generateByteCode(){
        expressionToByte(m_ast->m_lines->at(0));
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
        }

                
}

void ByteGen::decToCommand(Decleration* dec){
        string nameer=((Literal*)dec->m_name)->m_token->m_symbol;  
        expressionToByte(dec->m_value);
        toCommand("store "+((Literal*)dec->m_name)->m_token->m_symbol);

}

void ByteGen::binToCommand(BinOP* op){
        if(op->m_left->name() == "Literal" && op->m_right->name() == "Literal"){
                toCommand("push "+ ((Literal*)op->m_left)->m_token->m_symbol);
                toCommand("push "+((Literal*)op->m_right)->m_token->m_symbol);
                toCommand(Lexer::typeToString(op->m_op->m_type));
        }else if(op->m_left->name() == "Literal" && op->m_right->name() !="Literal"){
                expressionToByte(op->m_right);
                toCommand("push "+((Literal*)op->m_left)->m_token->m_symbol);
                toCommand(Lexer::typeToString(op->m_op->m_type));
        }else if(op->m_left->name() != "Literal" && op->m_right->name() =="Literal"){
                expressionToByte(op->m_left);
                toCommand("push "+((Literal*)op->m_right)->m_token->m_symbol);
                toCommand(Lexer::typeToString(op->m_op->m_type));
        }else {
                expressionToByte(op->m_left);
                expressionToByte(op->m_right);
                toCommand(Lexer::typeToString(op->m_op->m_type));

        }
}

void ByteGen::functionCallToByte(FunctionCall* call){
        string symb = call->m_name->m_token->m_symbol;
        for(int i=0; i< call->m_args->size();i++){
                expressionToByte(call->m_args->at(i));
        }
        m_lines->push_back("print");
}


#include "ByteGen.h"

ByteGen::ByteGen(Body* ast){
        m_ast = ast;
        m_lines = new vector<string>(); 
}

vector<string>* ByteGen::generateByteCode(){
        for(int i=0; i< m_ast->m_lines->size();i++)
                expressionToByte(m_ast->m_lines->at(i));
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
                if(type == TokenType::IDEN)
                        toCommand("load "+((Literal*)expr)->m_token->m_symbol);
                else 
                        toCommand("push "+((Literal*)expr)->m_token->m_symbol);
        }

                
}

void ByteGen::decToCommand(Decleration* dec){
        string nameer=((Literal*)dec->m_name)->m_token->m_symbol;  
        expressionToByte(dec->m_value);
        toCommand("store "+((Literal*)dec->m_name)->m_token->m_symbol);

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

#include "Linker.h"

Linker::Linker(string origin, Body* body){
    m_body = body;
    paths.insert(string(realpath(origin.c_str(),nullptr)));
}

void Linker::findAllPaths(Body* body){
    for(int i=0; i< body->m_lines->size(); i++){
        if(m_body->m_lines->at(i)->name() == "BuiltIn" && ((BuiltIn*)m_body->m_lines->at(i))->m_type->m_token->m_type == TokenType::IMPORT ){
             
        }
    }
}
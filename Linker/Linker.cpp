#include "Linker.h"
#include "ErrorThrower.h"
Linker::Linker(string origin, Body* body){
    m_body = body;
    paths.insert(string(realpath(origin.c_str(),nullptr)));
}

void Linker::findAllPaths(Body* body){
    for(int i=0; i< body->m_lines->size(); i++){
        if( isImport(body->m_lines->at(i))){
            Literal* lit =(Literal*)((BuiltIn*)body->m_lines->at(i))->m_value; 
            string origin = lit->m_token->m_symbol;
            string path;
            if(lit->m_token->m_type == TokenType::STRING){
                path =string(realpath(origin.c_str(),nullptr)); 
            }else if(lit->m_token->m_type == TokenType::IDEN){
                path = "/usr/lib/ablaze/"+origin+".abz";
            }
            ifstream ifile;
            ifile.open(path);
            if(!ifile){
                ErrorThrower::error(lit->m_token->m_line,"File "+path+" does not exist");
                ifile.close();
                i++;
                continue;
            }
            ifile.close();
 
            if(paths.find(path) == paths.end()){
                paths.insert(path);
                Body* body = readBody(path); 
                if(body != nullptr){
                    findAllPaths(body);
                    bods.push_back(body);
                }
            }
        }
    }
}

void Linker::linkFiles(){
    findAllPaths(m_body);
    for(int i=0; i< m_body->m_lines->size(); i++){
        if(isImport( m_body->m_lines->at(i) )){
           m_body->m_lines->erase(m_body->m_lines->begin()+i);
           i--; 
        }
    }
    for(int i=0; i<bods.size();i++){
        merge(m_body,bods[i]);
    }
}

bool Linker::isImport(Expression* expr){
    if(expr == nullptr||expr->name() != "BuiltIn")
        return false;
    BuiltIn* build =((BuiltIn*)expr); 
    return build->m_type->m_token->m_type == TokenType::IMPORT; 

}

void Linker::merge(Body* body, Body* second){
    for(int i=0; i< second->m_lines->size(); i++){
        if(!isImport(second->m_lines->at(i)))
            body->m_lines->push_back(second->m_lines->at(i));
    }
}

Body* Linker::readBody(string path){
    string wholeFile;
    string line;

    // read entire file
    ifstream reader(path);
   while ( getline ( reader , line ) ) {
        wholeFile += line+"\n";
    }
    reader.close();

  Lexer lexer(wholeFile);
    vector<Token*> tokens = lexer.readAllTokens();
    if(ErrorThrower::hasError){
        for(string s:*ErrorThrower::errors){
            cout<<s<<endl;
        }
        return nullptr;
    }

    ASTGen gen (tokens);
    Body* body = gen.generateAST();
  
    return body;
}
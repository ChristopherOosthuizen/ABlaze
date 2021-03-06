
#ifndef ABLAZE_Linker_h
#define ABLAZE_Linker_h
#include "ASTGen.h"
#include "AST.h"
#include <set>
#include <iostream>
#include <fstream>
#include "Lexer.h"
#include <filesystem>

class Linker{
    public:
        set<string> paths;
        vector<Body*> bods;
        Body* m_body;
        Linker(string origin, Body* body);
        void findAllPaths(Body* body);
        void linkFiles();
        void merge(Body* body,Body* second);
        Body* readBody(string path);
        bool isImport(Expression* expr);
};

#endif

#ifndef ABLAZE_Linker_h
#define ABLAZE_Linker_h
#include "ASTGen.h"
#include "AST.h"
#include <set>
#include <iostream>
#include <filesystem>

class Linker{
    public:
        set<string> paths;
        Body* m_body;
        Linker(string origin, Body* body);
        void findAllPaths(Body* body);
        Body* linkFiles();
};

#endif
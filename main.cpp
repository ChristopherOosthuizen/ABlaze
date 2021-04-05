/* Created by Christopher Oosthuizen 12/20/2020
 * The main executable of the ABlaze Programming language
 * That either starts a ABlaze repl if no input is given
 * Or executes a file if a file is given.
 */

#include <iostream>
#include <string>
#include <fstream>
#include "Lexer.h"
#include "ErrorThrower.h"
#include "ASTGen.h"
#include "SematicAn.h"
#include "ByteGen.h"
#include "Vm.h"
#include "ByteLexer.h"
#include "Linker.h"
using namespace std;

int version(){
        cout<<"ABlaze version: 0.1"<<endl;
        return 0;
}

int help(){
        cout<< "Help function not implemented yet"<<endl;
        return 0;
}

/* Starts a repl. or in other words
 * Continuously executes users input as the program runs
 */
int repl() {
    string input;
    do {
       cout << ">>";
       getline(cin , input);
       cout << input << endl;
    }while(input != "exit()");
    return 0;
}

int convertToByte(const string& wholeFile,const string& output, const string& path){
    ErrorThrower::m_content = wholeFile;
    Lexer lexer(wholeFile);
    vector<Token*> tokens = lexer.readAllTokens();
    if(ErrorThrower::hasError){
        for(string s:*ErrorThrower::errors){
            cout<<s<<endl;
        }
        return 1;
    }
    ASTGen gen (tokens);
    Body* body = gen.generateAST();
    if(ErrorThrower::hasError){
        for(string s:*ErrorThrower::errors){
            cout<<s<<endl;
        }
        return 1;
    }
    Linker linker(path,body);
    linker.linkFiles();
    if(ErrorThrower::hasError){
        for(string s:*ErrorThrower::errors){
            cout<<s<<endl;
        }
        return 1;
    }
 
    SematicAn an(body);
	 an.analize();
     if(ErrorThrower::hasError){
        for(string s:*ErrorThrower::errors){
            cout<<s<<endl;
        }
        return 1;
    }
    ByteGen byt(body);
    vector<string>* strs = byt.generateByteCode();
    ofstream filer;
    filer.open(output,std::ofstream::out | std::ofstream::trunc );
     for(string s:*strs){
        filer<<s<<endl;
    }
    filer.close();
   return 0;
}

/*
 * interprets ABlaze programs contained inside
 * files
 */
int file(char** argv, int argc ,string path,string  output,bool run) {
    string wholeFile;
    string line;

    // read entire file
    ifstream reader(path);

    if ( !reader.good() ) {
        perror(string("No file with name "+path+" Found").c_str());
        return 1;
    }
    while ( getline ( reader , line ) ) {
        wholeFile += line+"\n";
    }
    reader.close();
    if(!run)
        return convertToByte(wholeFile,output,path);
    ByteLexer lexer(wholeFile);
    vector<ByteToken*> tokens = lexer.readAllTokens();
    Vm vm(tokens);
	vm.loadArgs(argc,argv);
    vm.execute();
    return 0;
}


/*
 * Takes command prompt parameters
 * if there is no parameters open the repl
 * if there is one assume that the input is a file
 * if there are more throw a error.
 */
int main(int argc , char** argv) {
        if(argc == 1)
                return repl(); 
        string arg1 = argv[1];
        if(arg1 =="--help" || arg1=="-h")
                return help();
        else if(arg1 =="--version" || arg1=="-v")
                return version();
        string output = "a.laze";
        string input = argv[argc-1];
        if(arg1 == "-r"){
                return file(argv, argc,input,output,true);
        }
        if(arg1 == "-o"){
                output = argv[2];
        }
        return file(argv, argc,input,output,false);
}

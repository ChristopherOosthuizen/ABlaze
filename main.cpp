/* Created by Christopher Oosthuizen 12/20/2020
 * The main executable of the ABlaze Programming language
 * That either starts a ABlaze repl if no input is given
 * Or executes a file if a file is given.
 */

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

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

/*
 * interprets ABlaze programs contained inside
 * files
 */
int file(string path) {
    string wholeFile;
    string line;

    // read entire file
    ifstream reader(path);

    if ( !reader.good() ) {
        perror(string("No file with name "+path+" Found").c_str());
        return 1;
    }
    while ( getline ( reader , line ) ) {
        wholeFile += line;
    }
    reader.close();

	cout << wholeFile;
   return 0;
}

/*
 * Takes command prompt parameters
 * if there is no parameters open the repl
 * if there is one assume that the input is a file
 * if there are more throw a error.
 */
int main(int argc , char** argv) {
    if(argc ==1){
        return repl();
    }else if(argc ==2){
        return file(argv[1]);
    }
    perror("to many parameters");
    return 2;
}

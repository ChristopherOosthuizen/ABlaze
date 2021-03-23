---
title: Files
tage:
    - ablaze 
    - standards
    - imports
---
# Files
Working with multiple files is essential for a programming language so ablaze supports it though a few builtin functions
## Import
File: two.abz
```
void make(){
    println "hello";
}
```
File: one.abz
```
import "two.abz";
void main(){
    make();
}
```
Output
```
hello
```
## Import Library
Ablaze also features a standard library. To import it use a literal in the import bar instead of a string.
```
import math;
void main(){
    println min(14,99);
}
```
Output
```
14
```
## Read File
Being able to read a files contents is just as important as being able to use outside porgrams
```
void main(){
    string: lines = readFile("text.txt");
    for(int i=0; i< len lines; i++){
        println "line "+i+": "+lines[i];
    }
}
```
text.txt
```
hello
there 
```
Output:
```
line 0: hello
line 1: there 
```
## Write Files
You can also write to files
```
void main(){
    writeFile("hello.txt","hello");
}
```
hello.txt
```
hello
```
## Create Files
```
void main(){
    createFile("a");
    println exists("a");
}
```
Output
```
true
```
## Delete Files
```
void main(){
    createFile("a");
    deleteFile("a");
    println exists("a");
}
```
Output
```
false
```
Congratulations you have learned about files in ABlaze!
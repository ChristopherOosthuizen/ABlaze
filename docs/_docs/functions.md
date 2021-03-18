---
title: Functions 
tags: 
 - ABlaze 
 - DataTypes
 - Functions 
description: Teach the user how to use funuctions
---

# Functions
Like any good programming language ablaze can use functions to reuse code. 


## Declaration

To declare a function you need to specify the type, arguments and name.
```
int numPlus1(int num){
    return num+1; 
}
```
In the code above we declared a function named numPlus1 that took one parameter num.

## Calling
To call functions you need to invoke its name and specify the parameters
```
void printer(int i){
    println i;
}
void main(){
    printer(15);
}
```
Output
```
15
```

## Return 
As you can see from the above examples functions can return values you can opt out of having a return value if the function is void
```
void printTwoLines(){
    println "";
    println "";
}
int iPlus1(int i){
    return i+1;
}
void main(){
    printTwoLines();
    println iPlus1(12);
}
```
Output
```


13
```

## Builtin
As seen through the print and println functions there are also a number of builtin functions as follows

* print: prints the string provided
* println : prints the string provided and adds a newline at the end
* input: read input from the commandline
* readFile: returns the contexts of specified file
* writeFile: writes specified string to specified file
* createFile: creates file with name
* deleteFile: deletes specified file
* exists: checks to see if a file exists
* sin: returns the sine of a number
* cos: returns to cosine of a number
* tan: returns the tangent of a number

Congrats you have learned about functions in ABlaze!

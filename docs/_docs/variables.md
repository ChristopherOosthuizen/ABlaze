--- 
title: Variables
tags: 
- Ablaze 
- Variables 
- types
---
# Variables 
ABlaze has a builtin variable deceleration and system to use and assign variables

## Declaration
To declare variables you simply have to follow the following syntax
```
int main(){
    int i = 12;# with value
    int o; # without value
    println i;
    println o;
}
```
output
```
12
0
```

In this example we defined the variables i and o giving i the value of 12 and o no value. Because no value was given to o it was filled with nil which in the context of int equates to 0.

## Assignment 
You can also set values for variables that have already been set declared. As seen in the example below.
```
int main(){
    int i = 12;
    println i;
    i = 14;
    println i;
}
```
output 
```
12
14
```
There are also special assignment operators that can change the value of the variable
```
int main(){
    int i = 15;
    println i;
    i += 4;# addition
    println i;
    i -= 2;# subtraction
    println i;
    i *= 3; #multiplication
    println i;
    i /= 2; #divistion
    println i;
    i %=4; #modulue
    println i;
}
```
output
```
15
19
17
51
25
1
```

## Types 
As you may have seen throughout this tutorial ABlaze has a few different types well here are all the built-in types
```
int main(){
    int i = 12;
    double o = 13.4;
    char c= 'a';
    bool b= true;
    string s= "hello";
}
```
All the built-in data types have a hierarchy of conversion anything can be turned into a string anything. And anything can be cast as any of the other data types.

## Casting
If a data type is one type, but you want it in another a solution is to cast it. 
```
int main(){
    int i = (int)12.5;
    int o = (int)"5";
    char j = (char)c;
    string s = (string)true;
}

```
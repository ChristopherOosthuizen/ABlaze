---
title: Arrays
tags:
- ABLaze
- functions
- arrays
---
# Arrays
ABlaze comes with built-in dynamic array functionality 
## Initializing
There are two ways to initialize an array, with an array literal and with an array constructor
```
void main(){
    int: i = int[]; # with a array constructor
    int: lit = {12,14,15}; #With a array literal
}
```
## Appending
Obviously a list isn't very useful without a way to add values to it, so you can do so through the append function.
```
void main(){
    int: i = int[];
    append(i,14);
    append(i,15);
    append(i,12);
    println i;
}
```
Output
```
[14,15,12]
```

## Setting
you can also set values already set in a list
```
void main(){
    int: i = {12,14};
    i[0] = 3;
    i[1] = 10;
    println i;
}
```
Output
```
[3,10]
```

## Deleting
Say if you want to remove a value of array you can use the delete command
```
void main(){
    int: i = {15,18,19};
    delete(i,0);
    println i;
}
```
Output
```
[18,19]
```

## Length
If you want to get the length of an array you can use the len command
```
void main(){
    int: i = {1,4,5,6,6};
    println len(i);
}
```
Output
```
5
```

## Selecting
Now if you want to use a value in an array simply use the following syntax
```
void main(){
    int: i = {12,14};
    println i[0];
}
```
Output
```
12
```
## Strings 
Even though this is about lists most of the same syntax can be used on strings except for delete and append
```
void main(){
    string s = "hello";
    println s[0];
    println len(s);
}
```
Output
```
h
5
```

Congratulations you have learned about lists in Ablaze!
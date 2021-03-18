---
title: Control Structures
tags:
    - ABlaze
    - if
    - for 
    - while
    - structures
description: a overview of structures in ablaze
---
# Control Structures
A programming language that only executed linearly would not be very useful so to remedy this ABlaze has structures which include

* if
* while 
* for

# If
If statements are the simplest of all three of the structures. The execution goes as follows check the control statement, if the control statement is true execute the code.
```
void main(){
    if(true){
        println 12;
    }else{
        println 15;
    }
    if(false){
        println 19;
    }else{
        println 18;
    }
    if(false){
        println 5;   
    }else if(true){
        println 1;
    }
    if(true){
        println 0;
    }
}
```
Output
```
12
18
1
0
```

# While
While loops are very similar to if statements, the main difference is that they will continue executing while the control statement is true. 
```
void main(){
    int i =12;
    while(i>=0){
        i--;
    }
    println i;
}
```
Output
```
-1
```

# For
For loops are the most dynamic of the three, for loops have an initial statement that executes when the program enters the loop, an iterator statement that executes every iteration, and a control statement that determines if the for loop continues.
```
void main(){
    for(int i=0; i<5; i++){
        println i;
    }
}
```
Output
```
0
1
2
3
4
```

Congratulations you have learned about control structures in ABlaze!
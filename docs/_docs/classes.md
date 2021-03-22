---
title: Classes
tage:
    - structs
    - classes
    - ablaze
decription: A overview of how to use classes in the ablaze programming langauge
---
# Classes
Classes are essential if you want to do OOP. ABlaze supports them and some basic features that you may know.
## Basic
There is basic syntax to create objects in ABlaze
```
struct Pos{
    int x;
    int y;
}

void main(){
    Pos pos = new Pos;
    pos.x = 12;
    pos.y = 14;
    println pos.x+" "+pos.y;
}
```
Output:
```
12 14
```
## Constructors
It would be quite annoying to have to set the values manually like that every time so to compensate that there are constructors
```
struct Pos{
    int x;
    int y;
    void Pos(){
        x = 14;
        y = 15;
    }
    void Pos(int x, int y ){
        this.x = x;
        this.y = y;
    }
}
void main(){
    Pos one = new Pos();
    Pos two = new Pos(19,24);
    println one.x +" "+one.y;
    println two.x+" "+two.y;
}
```
Output
```
14 15
19 24
```

## Functions
ABlaze also supports functions within classes
```
struct Pos{
    int x;
    int y;
    void Pos(int x,int y){
        this.x = x;
        this.y = y;
    }
    void printPos(){
        println x +" "+y;
    }
}
void main(){
    Pos pos = new Pos(14,18);
    pos.printPos();
}
```
Output
```
14 18
```

## Static
Say if you want to keep a function in a class that doesn't modify the state of an object well you can by using static functions
```
struct Pos{
    static void printPos(int x,int y){
        println x+" "+y;
    }
}
void main(){
    Pos.printPos(12,15);
}
```
Output
```
12 15
```

Congratulations you learned about classes in ablaze.
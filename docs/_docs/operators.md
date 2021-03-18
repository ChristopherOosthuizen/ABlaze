---
title: Operators 
tags: 
 - ABlaze 
 - DataTypes
 - Math operators
description: tech the user how to use ablaze basic math operations
---


# Operators

## Math 
ABlaze for the most part has standard math operators so if you are familiar with Math operators you should be familiar with most of ABlaze's.

ABlaze has the following math operators

```
void main(){
    println 12+4; #addition
    println 12-4; #substraction
    println 12/4; #divition
    println 12*4; #multiplication
    println 12%4; #mod
    println 2**2; #exponent
    println 4//2; #radical
}
```
>Note as seen in the code above '#' specifies comments

The above code should yield the following output
```
16
8
3
48
0
4.00000
2.00000
```
As you should have seen from the output certain operations change the type from int to double.

### Pemdas
ABlaze also supports pemdas for order of operations so code like this
```
void main(){
    println 12+8*4**8/2+4-1
}
```
Will return
```
262159.000000
```

## Boolean
Like any good programming language ablaze supports boolean operations
ABlaze supports the following
```
void main(){
    println true&&true; # AND
    println true||true; # OR
    println true^true; # XOR
    println !true; #NOT
}
```
outputting 
```
true
true
false
false
```

I'm writing this with the assumption that ABlaze won't be anyone's first programming language but if you are unfamiliar with boolean operations.

* AND returns true if both operands are true
* OR returns true if either operands is true
* XOR returns true if either operand is true and false if they both are true
* NOT returns the opposite to if its true it returns false and if its true it will return true

## Equality 
ABlaze supports the basic equality operators
```
void main(){
    println 8 ==8;#EQUAL
    println 8!=8; #Not EQUAL
    println 12>4; #Greater than
    println 12<4; #Less than
    println 13>=14; #Greater than or equal
    println 13<=13; # Less than or equal
}
```
output
```
true
false
true
false
false
true
```
## Bitwise 
ABlaze also supports a few bitwise operators 
```
void main(){
    println 4<<4; #left shift
    println 32>>2; #Right Shift
    println 5&10; #bitwise and
    println 14|5; #bitwise or
    println 2^6; #bitwise xor
}
```
output
```
64
8
0
15
4
```
I don't have quite enough room to explain bitwise operations, but if you are unfamiliar you can read up on them [here](https://en.wikipedia.org/wiki/Bitwise_operation).


## String operations
As of now one kind of string operation exists which is addition that can also be used to switch the types of certain variables
```
void main(){
    println "Hello " +"there"; # string on string
    println "There are "+12+" students"; # string on other data type
}
```
output:
```
Hello there
There are 12 students
```

## Order of operations
The order of operations as of now goes as follows

1. ++ , -- , !
2. **, //
3. *, /, %
4. +, -
5. <<, >>
6. !=, ==, <, >, <=, >=, 
7. &, |, ^
8. &&, ||

Congratulations you have successfully learned about ablaze's operators!
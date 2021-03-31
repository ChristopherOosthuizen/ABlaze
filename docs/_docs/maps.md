---
title: Maps
tags: 
- ABLaze
- functions
- arrays
---
# Maps
ABlaze has builtin hashmaps for quick use
## Initializing
To initialize you need to specify both the key and value type. 
```
void main(){
	string:int maps = string[int];
	maps["hello"] = 12;
	maps["hi"] = 13;
	maps["right"] = 1;
	println maps["hello"]+ maps["hi"]+ maps["right"];
}
```
Output:
```
26
```

## Getting Keys and Vals
Maps also have built in functions to get keys and values
```
void main(){
	string:int maps = string[int];
	maps["hello"] = 12;
	maps["hi"] = 13;
	maps["right"] = 1;
	println maps.keys();
	println maps.vals();
}
```
Output:
```
[right,hi,hello]
[1,13,12]
```
As you can see you can get the keys and values and keys.

Congratulation you learned about maps in ABlaze!

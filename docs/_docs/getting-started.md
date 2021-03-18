---
title: Getting Started
tags: 
 - ABlaze 
 - Hello World
 - github
description: Getting started with ABlaze 
---

# Getting Started
Welcome the ABlaze documentation!
## Installation:

### 1. clone the repo
```
git clone https://github.com/ChristopherOosthuizen/ABlaze.git
cd ABlaze/
```

### 2. Run the Installation script 
```
./install.sh
```

### 3. install ABlaze
```
sudo make install
```
Congratulations you have successfully installed ABlaze!

## Hello World
HelloWorld.abz
```
void main(){
  println "Hello World!";
}
```
To run your Hello World
```
$ ablaze HelloWorld.abz
$ ablaze -r a.laze
Hello World!
```

Congraulations you can now put on your resume that you know ABlaze.
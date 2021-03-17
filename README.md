# ABlaze

![logo](logo.png)

A programming language for a busy programmer.

## About this project

This is a simple compiled programming language based off the C syntax but with certain shortcuts that most c like languages don't have.
This project builds to a byte code and the to nasm.

## Built with
This project is built as from scratch as possible in c++ with google test.

- [cmake](https://cmake.org/)
- [GoogleTest](https://github.com/google/googletest)

## Getting started

### Prerequisites 

For this project you need a version of gcc and cmake downloaded

### Installation
1. clone the repo
```
git clone https://github.com/ChristopherOosthuizen/ABlaze.git
```
2. go into the directory 
```
cd ABlaze/
```
3. run install script 
```
./install.sh
```
4. install ablaze
```
sudo make install
```

### Hello World 
1. Create a program

HelloWorld.abz
```
int main(){
    println "Hello World";
}
```
2. Compile Program 
```
ablaze HelloWorld.abz 
```
3. Run Program 
```
ablaze -r a.laze
```
## Usage

ABlaze is a multipurpose programming language with little to no emphasis in any direction. This can not be used for webdev that I have seen and can not be used for low level programming. But can be used for basic and scripting tasks. 

## Docs
Documentation and additional information can be found [here](https://christopheroosthuizen.github.io/ABlaze/).

## Contributing
If you want to contribute please do not hesitate to reach out at through one of the contact methods and make a pull request.

## Licence 
Distributed under the MIT License See `LICENCE` for more information.  

## Contact

Christopher Oosthuizen- Christopher.jan.oosthuizen@gmail.com

ABlaze - https://github.com/ChristopherOosthuizen/ABlaze.git

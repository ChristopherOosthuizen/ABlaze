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
3. Add the Google Test source
```
cd Google_tests/
git clone https://github.com/google/googletest.git
cd ../
```

### Running
1. Create a build dir
```
mkdir build/
```
2. Clone google test
```
cd Google_tests
git clone https://github.com/google/googletest.git
cd ../
```
3. build cmake project
```
cd build/
cmake ../
make
```
3. compile and run file 
```
./ABlaze [filename]
./ABlaze -r a.laze
```

## Usage

ABlaze is a multipurpose programming language with little to no enfisis in any direction. This can not be used for webdev that I have seen and can not be used for low level programming. But can be used for basic and scripting tasks. 



## Licence 
Distributed under the MIT License See `LICENCE` for more information.  

## Contact

Christopher Oosthuizen- Christopher.jan.oosthuizen@gmail.com

ABlaze - https://github.com/ChristopherOosthuizen/ABlaze.git

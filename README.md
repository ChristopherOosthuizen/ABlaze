# ABlaze

A programming language for a busy programmer.

## About this project

This is a simple compiled programming language based off the C sytax but with certain shotcuts that most c like languages dont have.
This project builds to a byte code and the to nasm.

## Why I built this

This project is exculisevly designed to test my own abilitys to make a programming language.
I wanted to
- Make a language(duh)
- Get a better understanding of the compile process
- Get better at c++
- Learn assembly

## Built with
This project is built as from scratch as possible in c++ with google test.

- [gcc](https://gcc.gnu.org/)
- [GoogleTest](https://github.com/google/googletest)

## Getting started

This project is not close enough to completion to be run as a lanaguage yet but to run tests. The Process goes as follows.

### Prerequisites 

For this project you need a version of gcc and cmake downloaded

### Installation
1. clone the repo
```
git clone https://github.com/ChristopherOosthuizen/ABlaze.git
```
2. go into the direcotry
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
2. build cmake project
```
cd build/
cmake ../
make
```
3. run tests
```
Google_tests/Google_Tests_run
```

## Usage

ABlaze is a multipurpose programming language with little to no enfisis in any direction. This can not be used for webdev that I have seen and can not be used for low level programming. But can be used for basic and scripting tasks. 

## Roadmap

For this project to be usable three more compontents need to be completed. 

1. Abstract syntax tree to byte code converter
2. Byte code to assembly converter
3. Command line app

## Licence 
Distributed under the MIT License See `LICENCE` for more infomation.  

## Contact

Christopher Oosthuizen- Christopher.jan.oosthuizen@gmail.com
ABlaze - https://github.com/ChristopherOosthuizen/ABlaze.git

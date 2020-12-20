cmake_minimum_required(VERSION 3.17)
project(ABlaze)

set(CMAKE_CXX_STANDARD 14)

add_executable(ABlaze main.cpp Lexer/Lexer.cpp Lexer/Lexer.h Lexer/Token.h Lexer/TokenType.h)
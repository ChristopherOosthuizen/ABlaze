cmake_minimum_required(VERSION 3.17)
project(ABlaze)

set(CMAKE_CXX_STANDARD 14)
add_executable(ABlaze main.cpp   ErrorThrower/ErrorThrower.h ErrorThrower/ErrorThrower.cpp  )

add_subdirectory(Google_tests)
add_subdirectory(Lexer)
add_subdirectory(ErrorThrower)

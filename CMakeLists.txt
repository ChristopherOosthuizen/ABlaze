cmake_minimum_required(VERSION 3.16)

project(ABlaze)

set(CMAKE_BUILD_TYPE Debug)
add_executable(ABlaze main.cpp   ErrorThrower/ErrorThrower.h ErrorThrower/ErrorThrower.cpp  )

add_subdirectory(Google_tests)
add_subdirectory(Lexer)
add_subdirectory(ErrorThrower)
add_subdirectory(AST)
add_subdirectory(Sematic)
add_subdirectory(ByteCodeGenerator)
add_subdirectory(VM)

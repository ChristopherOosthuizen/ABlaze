cmake_minimum_required(VERSION 3.16)

project(ablaze)

set(CMAKE_BUILD_TYPE Debug)
add_executable(ablaze main.cpp   ErrorThrower/ErrorThrower.h ErrorThrower/ErrorThrower.cpp  )

add_subdirectory(Google_tests)
add_subdirectory(Lexer)
add_subdirectory(ErrorThrower)
add_subdirectory(AST)
add_subdirectory(Sematic)
add_subdirectory(ByteCodeGenerator)
add_subdirectory(Linker)
add_subdirectory(VM)
include_directories(Lexer ErrorThrower AST Sematic ByteCodeGenerator VM Linker)
target_link_libraries(ablaze Lexer ErrorThrower AST Sematic ByteCodeGenerator VM Linker)

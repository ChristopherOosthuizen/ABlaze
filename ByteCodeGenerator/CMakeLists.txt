add_library(ByteCodeGenerator ByteGen.cpp ByteGen.h)

target_include_directories(ByteCodeGenerator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../Lexer ../ErrorThrower ../AST)

add_library(AST AST.h AST.cpp  ASTGen.h ASTGen.cpp )

target_include_directories(AST PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../Lexer ../ErrorThrower)



add_library(Lexer Lexer.h Lexer.cpp Token.h TokenType.h)

target_include_directories(Lexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../ErrorThrower)

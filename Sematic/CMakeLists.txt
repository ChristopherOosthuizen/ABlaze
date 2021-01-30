add_library(Sematic SematicAn.h SematicAn.cpp )

target_include_directories(Sematic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../AST ../ErrorThrower ../Lexer  )


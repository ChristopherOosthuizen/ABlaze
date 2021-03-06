add_library(Linker Linker.h Linker.cpp )

target_include_directories(Linker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../AST ../ErrorThrower ../Lexer  )

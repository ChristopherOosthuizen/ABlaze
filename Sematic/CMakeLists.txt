add_library(Sematic SematicAn.cpp SematicAn.h)

target_include_directories(Sematic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../AST ../ErrorThrower)


Project(Google_tests)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../Lexer ../ErrorThrower ../ByteCodeGenerator ../VM)

add_executable(Google_Tests_run   ASTBasicTest.cpp ASTTest.cpp ) 

target_link_libraries(Google_Tests_run gtest gtest_main Lexer ErrorThrower AST VM)

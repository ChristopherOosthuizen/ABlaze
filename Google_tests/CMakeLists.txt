Project(Google_tests)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../Lexer ../ErrorThrower ../Sematic ../ByteCodeGenerator)

add_executable(Google_Tests_run LexerTest.cpp ErrorThrowerTest.cpp ASTTest.cpp BaseASTTest.cpp ASTErrorTest ASTGeneralTests.cpp SematicAnTest.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main Lexer ErrorThrower AST Sematic ByteCodeGenerator)

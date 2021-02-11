add_library(VM  ByteLexer.h ByteLexer.cpp ByteType.h Vm.cpp Vm.h  ByteType.h ByteToken.h)

target_include_directories(VM PUBLIC ${CMAKE_CURREN_SOURCE_DIR})

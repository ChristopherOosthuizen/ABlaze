add_library(VM Vm.h Vm.cpp ByteType.h ByteToken.h)

target_include_directories(VM PUBLIC ${CMAKE_CURREN_SOURCE_DIR})

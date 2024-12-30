cmake_minimum_required(VERSION 3.5.0)
project(socket)

include_directories(C:/Users/Menar/source/C++/vcpkg/installed/x64-windows/include)
include_directories(server)

link_directories(C:/Users/Menar/source/C++/vcpkg/installed/x64-windows/lib)

add_executable(socket main_server.cpp server/server.cpp server/crypto/crypto.cpp)

find_package(OpenSSL REQUIRED)
find_package(aws-cpp-sdk-core REQUIRED)


target_link_libraries(socket OpenSSL::SSL)
target_link_libraries(socket OpenSSL::Crypto)
target_link_libraries(socket fmt)
target_link_libraries(socket aws-cpp-sdk-core)

#target_include_directories(socket aws-cpp-sdk-core)

add_compile_options("/utf-8")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

include(CTest)
enable_testing()


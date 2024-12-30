cmake_minimum_required(VERSION 3.5.0)
project(socket)

include_directories(C:/Users/Menar/source/C++/vcpkg/installed/x64-windows/include)
link_directories(C:/Users/Menar/source/C++/vcpkg/installed/x64-windows/lib)

add_executable(socket main.cpp)

find_package(OpenSSL REQUIRED)

target_link_libraries(socket OpenSSL::SSL)
target_link_libraries(socket OpenSSL::Crypto)
target_link_libraries(socket fmt)


add_compile_options(/utf-8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

include(CTest)
enable_testing()


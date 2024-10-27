cmake_minimum_required(VERSION 3.28)
project(smalllang)

find_package(GTest REQUIRED)

add_library(smalllang_lib STATIC token.cpp token.hpp lexer.hpp)
add_executable(smalllang smalllang.cpp)
add_executable(smalllang_test smalllang_test.cpp)
target_link_libraries(smalllang PRIVATE smalllang_lib)
target_link_libraries(smalllang_test PRIVATE smalllang_lib GTest::GTest)
target_include_directories(smalllang_test PRIVATE GTest::GTest)
target_compile_features(smalllang PRIVATE cxx_std_23)


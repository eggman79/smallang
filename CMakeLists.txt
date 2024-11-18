cmake_minimum_required(VERSION 3.28)
project(smallang)

find_package(GTest REQUIRED)

add_library(smallang_lib STATIC ast.hpp token.cpp token.hpp lexer.hpp parser.cpp parser.hpp id_cache.hpp id_index.hpp ast_node_index.hpp) 
add_executable(smallang smallang.cpp)
add_executable(smallang_test tests.cpp)
target_link_libraries(smallang PRIVATE smallang_lib)
target_link_libraries(smallang_test PRIVATE smallang_lib GTest::GTest)
target_include_directories(smallang_test PRIVATE GTest::GTest)
target_compile_features(smallang PRIVATE cxx_std_23)
add_compile_options(smallang_test PRIVATE -fsanitize=address)
add_link_options(smallang_test PRIVATE -fsanitize=address)

enable_testing()
add_test(test smallang_test)

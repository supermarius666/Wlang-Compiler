cmake_minimum_required(VERSION 3.8)
project(wlang_compiler)

add_executable(wlang
    # test/main.cpp
    test/print_test.cpp
    source/Lexer/Lexer.cpp
    source/Lexer/Token.cpp
)

target_include_directories(wlang
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

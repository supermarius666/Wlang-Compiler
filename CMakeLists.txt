cmake_minimum_required(VERSION 3.8)
project(wlang_compiler)

add_executable(wlang
    # test/main.cpp
    # source/Lexer/Lexer.cpp
    # source/Lexer/Token.cpp
    source/CodeGen/main.c
    source/CodeGen/memory.c
    source/CodeGen/chunk.c
    source/CodeGen/debug.c
    source/CodeGen/value.c
    source/VM/vm.c
)

target_include_directories(wlang
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

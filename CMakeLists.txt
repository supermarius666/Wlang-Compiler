cmake_minimum_required(VERSION 3.8)
project(wlang_compiler)

add_executable(wlang
    source/CodeGen/main.c
    source/CodeGen/memory.c
    source/CodeGen/chunk.c
    source/CodeGen/debug.c
    source/CodeGen/value.c
    source/CodeGen/object.c
    source/CodeGen/table.c
    source/CodeGen/byte.c
    source/VM/vm.c
    source/VM/compiler.c
    source/Lexer/scanner.c
)

if (UNIX)
  target_link_libraries(wlang m)
endif()

target_include_directories(wlang
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
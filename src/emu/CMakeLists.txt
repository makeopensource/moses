cmake_minimum_required(VERSION 4.0)
project(moses-core C)

add_library(moses-core)

target_include_directories(moses-core PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_sources(moses-core
        PRIVATE
        emu.c
        mem.c
        cpu.c
        rom_parser.c
)

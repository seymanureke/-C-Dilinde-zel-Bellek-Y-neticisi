cmake_minimum_required(VERSION 4.0)
project(untitled17 C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled17 custom_allocator.c
        custom_allocator.h
        main.c)

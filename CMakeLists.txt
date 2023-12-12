cmake_minimum_required(VERSION 3.26)
project(CompiladorBasico)

set(CMAKE_CXX_STANDARD 17)

add_executable(CompiladorBasico main.cpp
        Funciones.cpp
        Funciones.h
        Compiler.cpp
        Compiler.h)

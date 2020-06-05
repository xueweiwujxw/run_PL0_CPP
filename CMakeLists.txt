cmake_minimum_required(VERSION 3.16)
project(run_PL0)

set(CMAKE_CXX_STANDARD 14)

add_executable(run_PL0 main.cpp block.h getsym.h interpret.h)
cmake_minimum_required(VERSION 3.13)
project(BPlusTree)

set(CMAKE_CXX_STANDARD 17)

add_executable(BPlusTree main.cpp BPlusTree.h Functions.h output.h "Work Reports.h" Pair.h)
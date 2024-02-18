cmake_minimum_required(VERSION 3.25)
project(search)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(search
        main.cpp
        mySearch.cpp
        mySearch.h)

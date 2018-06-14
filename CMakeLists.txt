cmake_minimum_required(VERSION 3.1.0)
project(wlib-tmp)

file(GLOB_RECURSE WLIB_TMP_FILES
    include/*
    include/wlib/*.h
    )

add_executable(tests ${WLIB_TMP_FILES} tests/main.cpp)
target_include_directories(tests PRIVATE include)

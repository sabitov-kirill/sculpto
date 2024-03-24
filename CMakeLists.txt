cmake_minimum_required(VERSION 3.20)
project(sclupto_renewed)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(utils)
add_subdirectory(window)

find_library(GDIPLUS gdiplus)

project(sclupto_renewed_main)
add_executable(sclupto_renewed_main main.cpp)
target_link_libraries(sclupto_renewed_main PRIVATE utils window gdiplus)
target_precompile_headers(sclupto_renewed_main PRIVATE pch.h)

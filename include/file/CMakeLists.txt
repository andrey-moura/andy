#Require a minimum version
cmake_minimum_required(VERSION 3.10)

project(uva-file)

add_library(uva-file
	${CMAKE_CURRENT_LIST_DIR}/src/file.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR})
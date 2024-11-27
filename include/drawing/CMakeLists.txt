#Require a minimum version
cmake_minimum_required(VERSION 3.10)

project(uva-drawing)

add_library(uva-drawing
	${CMAKE_CURRENT_LIST_DIR}/src/drawing.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/palette.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/colormap.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR})
#Require a minimum version
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(uva-xml)

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_library(uva-xml OBJECT
    ${CMAKE_CURRENT_LIST_DIR}/src/xml.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/schema.cpp
)

# uva_tests_configure(uva-xml)

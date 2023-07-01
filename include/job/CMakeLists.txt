#Require a minimum version
cmake_minimum_required(VERSION 3.10)

project(uva-job)

add_library(uva-job
	${CMAKE_CURRENT_LIST_DIR}/src/job.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set(JOB_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
include(${JOB_ROOT_DIR}/job.cmake)

target_link_libraries(uva-job uva-file)

# tools
include("${CMAKE_CURRENT_LIST_DIR}/job/CMakeLists.txt")

# samples

#Tests

#Create tests for parent project
#cspec_configure("uva-job" ${CMAKE_CURRENT_LIST_DIR} "uva-job")
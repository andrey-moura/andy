#Require a minimum version
cmake_minimum_required(VERSION 3.16)

project(uva)

# Commum libs
find_package(Threads)

# Optional libs
# binary, networking
find_package(OpenSSL)

set(UVA_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# cspec must be included before ALL projects
include("${CMAKE_CURRENT_LIST_DIR}/include/cspec/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/include/console/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/include/networking/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/include/json/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/include/binary/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/include/file/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/include/faker/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/include/routing/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/include/string/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/include/diagnostics/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/include/core/CMakeLists.txt")

include("${CMAKE_CURRENT_LIST_DIR}/include/networking/networking.cmake")

if(PROJECT_IS_TOP_LEVEL)
  message(STATUS "We're building uva. The project has been included.")
  include("${CMAKE_CURRENT_LIST_DIR}/uva/CMakeLists.txt")
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)
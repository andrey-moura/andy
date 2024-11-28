#Require a minimum version
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

project(uva)

# Commum libs
find_package(Threads)

# Optional libs
# binary, networking
find_package(OpenSSL)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__UVA_DEBUG__")

set(UVA_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(UVA_PROJECT_FOLDER ${CMAKE_SOURCE_DIR})

macro (uva_library uva_library_name)
    include(${CMAKE_CURRENT_LIST_DIR}/include/${uva_library_name}/CMakeLists.txt)
endmacro ()

uva_library(core)
uva_library(string)
uva_library(json)
uva_library(file)
uva_library(console)
uva_library(binary)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include/fmt/include)
# Copy the folder src/icons to the build directory

file(GLOB UVA_RESOURCES "${CMAKE_SOURCE_DIR}/uva-resources/*")

foreach(file ${UVA_RESOURCES})
    if(WIN32)
        configure_file(${file} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE} COPYONLY)
    else()
        configure_file(${file} ${CMAKE_BINARY_DIR} COPYONLY)
    endif()
endforeach()
#Require a minimum version
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

set(ANDY_PROJECT_VERSION 1.0.0)

project(uva)

# Commum libs
find_package(Threads)

# Optional libs
# binary, networking
find_package(OpenSSL)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__UVA_DEBUG__")

set(UVA_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(UVA_MACROS_CMAKE ${UVA_ROOT_DIR}/uva.cmake)

set(UVA_PROJECT_FOLDER ${CMAKE_SOURCE_DIR})

include(${UVA_MACROS_CMAKE})

if(UVA_IS_TOP_LEVEL)
    message(STATUS "Uva is the top level project")

    uva_library(core)
    uva_library(string)
    uva_library(json)
    uva_library(file)
    uva_library(console)
    uva_library(binary)
    uva_library(xml)
    uva_library(uva-tests)
    uva_library(uva-drawing)
    uva_library(uva-widgets)
endif()

# FMT is mandatory under Linux
if(UNIX)
    execute_process(
        COMMAND git submodule update include/fmt
        WORKING_DIRECTORY ${UVA_ROOT_DIR}
        RESULT_VARIABLE RESULT
        OUTPUT_VARIABLE OUTPUT
        ERROR_VARIABLE ERROR
    )
endif()

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

install(FILES     ${CMAKE_CURRENT_LIST_DIR}/include/uva.hpp DESTINATION include)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/uva     DESTINATION include)

install(EXPORT andyTargets
    FILE andyTargets.cmake
    NAMESPACE andy::
    DESTINATION lib/cmake/andy
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/andyConfigVersion.cmake
    VERSION ${ANDY_PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/andyConfig.cmake.in
    ${CMAKE_BINARY_DIR}/andyConfig.cmake
    @ONLY
)

install(FILES
    ${CMAKE_BINARY_DIR}/andyConfig.cmake
    ${CMAKE_BINARY_DIR}/andyConfigVersion.cmake
    DESTINATION lib/cmake/andy
)
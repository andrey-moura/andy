#Require a minimum version
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)

# read VERSION
file(READ ${CMAKE_CURRENT_LIST_DIR}/VERSION ANDY_PROJECT_VERSION)

project(andy VERSION ${ANDY_PROJECT_VERSION})

# Common libs
find_package(Threads)

# Optional libs
# binary, networking
find_package(OpenSSL)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__UVA_DEBUG__")
add_compile_definitions(ANDY_VERSION="${ANDY_PROJECT_VERSION}")
add_compile_definitions(ANDY_BUILD_TYPE="${CMAKE_BUILD_TYPE}")
add_compile_definitions(ANDY_CPP_VERSION="${CMAKE_CXX_STANDARD}")
add_compile_definitions(ANDY_COMPILER="${CMAKE_CXX_COMPILER_ID}")

set(UVA_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(UVA_MACROS_CMAKE ${UVA_ROOT_DIR}/uva.cmake)

set(UVA_PROJECT_FOLDER ${CMAKE_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include/fmt/include)

# Extracts the folder from CMAKE_SOURCE_DIR
get_filename_component(ANDY_TOP_LEVEL_FOLDER ${CMAKE_SOURCE_DIR} NAME)

if(${ANDY_TOP_LEVEL_FOLDER} STREQUAL "andy")
    set(ANDY_IS_TOP_LEVEL ON)
else()
    set(ANDY_IS_TOP_LEVEL OFF)
endif()

if(ANDY_IS_TOP_LEVEL)
    add_executable(andy-pm
        ${CMAKE_CURRENT_LIST_DIR}/src/andy_pm.cpp
    )
    add_executable(andy-pkgr
        ${CMAKE_CURRENT_LIST_DIR}/src/andy_pkgr.cpp
    )
endif()

# Copy the folder src/icons to the build directory
file(GLOB UVA_RESOURCES "${CMAKE_SOURCE_DIR}/uva-resources/*")

foreach(file ${UVA_RESOURCES})
    if(WIN32)
        configure_file(${file} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE} COPYONLY)
    else()
        configure_file(${file} ${CMAKE_BINARY_DIR} COPYONLY)
    endif()
endforeach()

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/uva     DESTINATION include)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/include/uva.hpp     DESTINATION include)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/cmake/andy.cmake DESTINATION lib/cmake/andy/cmake)
if(ANDY_IS_TOP_LEVEL)
    install(TARGETS andy-pm       EXPORT andyTargets RUNTIME DESTINATION bin)
    install(TARGETS andy-pkgr     EXPORT andyTargets RUNTIME DESTINATION bin)
    install(EXPORT andyTargets
        FILE andyTargets.cmake
        NAMESPACE andy::
        DESTINATION lib/cmake/andy
    )
endif()
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
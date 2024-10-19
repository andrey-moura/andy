#Require a minimum version
cmake_minimum_required(VERSION 3.16)

project(uva)

# Commum libs
find_package(Threads)

# Optional libs
# binary, networking
find_package(OpenSSL)

set(UVA_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

IF(DEFINED ENV{UVA_ENV})
    SET(UVA_ENV ENV{UVA_ENV})
ELSE()
    SET(UVA_ENV "development")
ENDIF()

MESSAGE(STATUS "ENV: ${UVA_ENV}")

set(UVA_PROJECT_FOLDER ${CMAKE_SOURCE_DIR})
set(UVA_DOWNLOAD_FOLDER ${UVA_PROJECT_FOLDER}/include)

if(NOT EXISTS "${UVA_DOWNLOAD_FOLDER}")
    file(MAKE_DIRECTORY ${UVA_DOWNLOAD_FOLDER})
endif()

execute_process (
    COMMAND git init
    WORKING_DIRECTORY ${UVA_PROJECT_FOLDER}
    OUTPUT_VARIABLE git_log
    RESULT_VARIABLE git_result
)

macro (uva_library uva_library_name)

    set(uva_library_folder "include/${uva_library_name}")
    set(uva_library_path "${uva_library_folder}/CMakeLists.txt")

    if(EXISTS "${UVA_PROJECT_FOLDER}/${uva_library_path}")
        message("Found uva_library ${uva_library_name} at: ${UVA_PROJECT_FOLDER}/${uva_library_path}")
    else()
        message(STATUS "uva_library ${uva_library_name} not found at '${UVA_PROJECT_FOLDER}/${uva_library_path}'. Will submodule it at '${uva_library_folder}'...")
        
        execute_process (
            COMMAND git submodule add --force git@github.com:andrey-moura/${uva_library_name}.git ${uva_library_folder}
            WORKING_DIRECTORY ${UVA_PROJECT_FOLDER}
            OUTPUT_VARIABLE git_log
            RESULT_VARIABLE git_result
        )

        message(STATUS "GIT output ${git_log} ${git_error} with exit code ${git_result}")

        execute_process (
            COMMAND git checkout ${UVA_ENV}
            WORKING_DIRECTORY ${UVA_PROJECT_FOLDER}/${uva_library_folder}
            OUTPUT_VARIABLE git_log
            RESULT_VARIABLE git_result
        )

        message(STATUS "GIT output ${git_log} ${git_error} with exit code ${git_result}")

        execute_process (
            COMMAND git submodule init 
            WORKING_DIRECTORY ${UVA_PROJECT_FOLDER}/${uva_library_folder}
            OUTPUT_VARIABLE git_log
            RESULT_VARIABLE git_result
        )

        message(STATUS "GIT output ${git_log} ${git_error} with exit code ${git_result}")

        execute_process (
            COMMAND git submodule update
            WORKING_DIRECTORY ${UVA_PROJECT_FOLDER}/${uva_library_folder}
            OUTPUT_VARIABLE git_log
            RESULT_VARIABLE git_result
        )

        message(STATUS "GIT output ${git_log} ${git_error} with exit code ${git_result}")
    endif()

    include("${uva_library_path}")
endmacro ()

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)

# Copy the folder src/icons to the build directory

file(GLOB UVA_RESOURCES "${CMAKE_SOURCE_DIR}/uva-resources/*")

foreach(file ${UVA_RESOURCES})
    if(WIN32)
        configure_file(${file} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE} COPYONLY)
    else()
        configure_file(${file} ${CMAKE_BINARY_DIR} COPYONLY)
    endif()
endforeach()
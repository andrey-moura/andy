#Require a minimum version
cmake_minimum_required(VERSION 3.16)

project(uva)

# Commum libs
find_package(Threads)

# Optional libs
# binary, networking
find_package(OpenSSL)

set(UVA_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

MESSAGE(STATUS $ENV{UVA_ENV})

IF(DEFINED ENV{UVA_ENV})
    SET(UVA_ENV ENV{UVA_ENV})
ELSE()
    SET(UVA_ENV "development")
ENDIF()

macro (juice juice_name)
    set(juice_folder "${CMAKE_CURRENT_LIST_DIR}/include/${juice_name}")
    set(juice_path "${juice_folder}/CMakeLists.txt")

    if(EXISTS "${juice_path}")
        message("Found juice ${juice_name} at: ${juice_path}")
        include("${juice_path}")

        message(STATUS "Configuring all submodules as ${UVA_ENV}")

        # execute_process (
        #     COMMAND bash -c "git -C '${juice_folder}' submodule foreach 'git checkout ${UVA_ENV}'"
        #     OUTPUT_VARIABLE outVar
        # )
    endif()
endmacro ()

# cspec must be included before ALL projects
juice("cspec")
juice("console")
juice("networking")
juice("json")
juice("binary")
juice("file")
juice("faker")
juice("routing")
juice("string")
juice("diagnostics")
juice("core")
juice("drawing")
juice("wx")
juice("job")
juice("database")

if(PROJECT_IS_TOP_LEVEL)
  message(STATUS "We're building uva. The project has been included.")
  include("${CMAKE_CURRENT_LIST_DIR}/uva/CMakeLists.txt")
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)

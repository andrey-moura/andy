macro (uva_library uva_library_name)
    set(LIBRARY_FOLDER ${UVA_ROOT_DIR}/include/${uva_library_name})
    
    if(EXISTS ${LIBRARY_FOLDER}/CMakeLists.txt)
        message(STATUS "${LIBRARY_FOLDER}/CMakeLists.txt exists")
    else()
        execute_process(
            COMMAND git submodule update include/${uva_library_name}
            WORKING_DIRECTORY ${UVA_ROOT_DIR}
            RESULT_VARIABLE RESULT
            OUTPUT_VARIABLE OUTPUT
            ERROR_VARIABLE ERROR
        )

        message(STATUS "Cloning ${uva_library_name} library: ${OUTPUT}")
        message(STATUS "Cloning ${uva_library_name} library: ${ERROR}")
        message(STATUS "Cloning ${uva_library_name} library: ${RESULT}")

        if(NOT ${RESULT} EQUAL 0)
            message(FATAL_ERROR "Error cloning ${uva_library_name} library: ${ERROR}")
        endif()
    endif()

    include(${LIBRARY_FOLDER}/CMakeLists.txt)
endmacro ()
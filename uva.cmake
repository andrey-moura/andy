macro (uva_library uva_library_name)
    set(LIBRARY_FOLDER ${UVA_ROOT_DIR}/include/${uva_library_name})
    
    if(NOT EXISTS ${LIBRARY_FOLDER})
        execute_process(
            COMMAND git submodule update include/${uva_library_name}
            WORKING_DIRECTORY ${UVA_ROOT_DIR}
            RESULT_VARIABLE RESULT
            OUTPUT_VARIABLE OUTPUT
            ERROR_VARIABLE ERROR
        )

        if(NOT ${RESULT} EQUAL 0)
            message(FATAL_ERROR "Error cloning ${uva_library_name} library: ${ERROR}")
        endif()
    endif()

    include(${LIBRARY_FOLDER}/CMakeLists.txt)
endmacro ()
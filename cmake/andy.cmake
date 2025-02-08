macro (uva_library uva_library_name)
    execute_process(
        COMMAND andy-pm ${uva_library_name}
    )
endmacro ()
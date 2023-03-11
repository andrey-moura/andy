#Require a minimum version
cmake_minimum_required(VERSION 3.10)

project(uva)

include(${CONSOLE_ROOT_DIR}/console.cmake)

add_console_app(uva)

target_link_libraries(uva uva-console uva-core uva-routing)

add_custom_command(TARGET uva 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:uva> ${CMAKE_SOURCE_DIR}/bin/uva)


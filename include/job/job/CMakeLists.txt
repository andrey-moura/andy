#Require a minimum version
cmake_minimum_required(VERSION 3.10)

project(uva-job)

include(${CONSOLE_ROOT_DIR}/console.cmake)

add_console_app(job)

target_link_libraries(job uva-core uva-job uva-console uva-routing)

add_custom_command(TARGET job 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:job> ${CMAKE_SOURCE_DIR}/bin/job)


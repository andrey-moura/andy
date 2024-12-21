project(uva-widgets)

add_library(uva-widgets
    ${CMAKE_CURRENT_LIST_DIR}/src/widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/layout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/text.cpp
)

add_executable(uva-renderer
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_link_libraries(uva-widgets -lSDL2 uva-xml uva-binary -lSDL2_ttf -lSDL2_gfx)

# uva_tests_configure(uva-widgets)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
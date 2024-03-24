project(render)

add_library(render INTERFACE
        primitives/render_context.h)

target_link_libraries(render INTERFACE utils)
target_include_directories(render INTERFACE ${PROJECT_SOURCE_DIR})
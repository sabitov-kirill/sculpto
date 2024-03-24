project(opengl-backend)

find_package(OpenGL REQUIRED)

add_library(opengl-backend SHARED gl_render_context.cpp gl_render_context.h)

target_link_libraries(opengl-backend PRIVATE render utils window OpenGL::GL)
target_include_directories(opengl-backend PRIVATE ${PROJECT_SOURCE_DIR})
project(window)

add_library(window INTERFACE
    base_window.h
    windows_window.h)

target_link_libraries(window INTERFACE utils)
target_include_directories(window INTERFACE ${PROJECT_SOURCE_DIR})
project(utils)

add_library(utils STATIC
        platform.h
        logger/logger.cpp
        logger/logger.h
        logger/console_logger.cpp
        logger/console_logger.h
        lassert.cpp
        lassert.h
        exception.h)

target_include_directories(utils PUBLIC ${PROJECT_SOURCE_DIR})
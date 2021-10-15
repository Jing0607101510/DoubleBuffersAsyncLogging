cmake_minimum_required(VERSION 3.4.1)

project(async_logging)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

aux_source_directory(${PROJECT_SOURCE_DIR} src)

add_library(${PROJECT_NAME} STATIC ${src})

add_subdirectory(test)
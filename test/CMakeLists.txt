cmake_minimum_required(VERSION 3.4.1)

project(logger_test)

aux_source_directory(. src)

add_executable(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME} async_logging)
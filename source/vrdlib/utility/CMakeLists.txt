cmake_minimum_required(VERSION 2.8.11)
get_filename_component(ABSOLUTE_PROJECT_PATH . ABSOLUTE)
get_filename_component(PROJECT_DIRECTORY_NAME ${ABSOLUTE_PROJECT_PATH} NAME)
project(vrdlib.utility)

aux_source_directory(source PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_link_libraries(${PROJECT_NAME} log4cxx)

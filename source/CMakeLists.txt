cmake_minimum_required(VERSION 3.10)
project(vrd)

IF(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
ENDIF()

IF(CMAKE_BUILD_TYPE STREQUAL "Coverage")
  set(CMAKE_BUILD_TYPE "Debug")
  add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
  link_libraries(gcov)
  message(STATUS "Configure build for coverage analysis")
ENDIF()

add_compile_options(--std=c++17)
add_compile_options(-Wall)
add_compile_options(-Werror)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_library(Log4cxx REQUIRED log4cxx) # There is no conan package
SET(Log4cxx_LIBRARIES log4cxx)         # There is no findpackage

add_subdirectory(vrdlib)
add_subdirectory(vrdextractor)

SET(CPACK_PACKAGE_NAME "vrdextractor")
SET(CPACK_PACKAGE_DESCRIPTION "Tool to extract virtual recipe data from Canon CRW/CR2 files and to write/merge it to XMP sidecar files used by darktable")
SET(CPACK_PACKAGE_CONTACT "Sascha Lumma")
SET(CPACK_PACKAGE_VERSION "0.1.0")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_FILE_NAME "vrdextractor-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE")
SET(CPACK_RESOURCE_FILE_README  "${CMAKE_CURRENT_SOURCE_DIR}/../README.md")

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "utils")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
 
INCLUDE(CPack) 

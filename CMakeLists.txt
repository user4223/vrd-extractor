cmake_minimum_required(VERSION 3.10)
project(vrd)

set(CONAN_DISABLE_CHECK_COMPILER ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json to make auto-complete working in vscode
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden) # Due to static linking, we get visibility warning otherwiese, since libs are using hidden und default here would lead to public
set(CMAKE_CXX_STANDARD 20)
set(SMAKE_CXX_STANDARD_REQUIRED ON)

IF(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
ENDIF()

IF(CMAKE_BUILD_TYPE STREQUAL "Coverage")
  set(CMAKE_BUILD_TYPE "Debug")
  add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
  link_libraries(gcov)
  message(STATUS "Configure build for coverage analysis")
ENDIF()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(log4cxx REQUIRED)
find_package(exiv2 REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

add_subdirectory(source/vrdlib)
add_subdirectory(source/vrdextractor)

SET(CPACK_PACKAGE_NAME "vrdextractor")
SET(CPACK_PACKAGE_DESCRIPTION "Tool to extract virtual recipe data from Canon CRW/CR2 files and to write/merge it to XMP sidecar files used by darktable")
SET(CPACK_PACKAGE_CONTACT "user4223")
SET(CPACK_PACKAGE_VERSION "0.1.0")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_FILE_NAME "vrdextractor-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_RESOURCE_FILE_README  "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS OFF)
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "utils")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
 
INCLUDE(CPack) 

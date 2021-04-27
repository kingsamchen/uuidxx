cmake_minimum_required(VERSION 3.16)

if(POLICY CMP0076)
  cmake_policy(SET CMP0076 NEW)
endif()

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(UUIDXX_NOT_SUBPROJECT ON)
endif()

project(uuidxx)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(UUIDXX_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})

  set_directory_properties(PROPERTIES VS_STARTUP_PROJECT "uuidxx_test")
endif()

# TODO: Add options here.

set(UUIDXX_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(UUIDXX_CMAKE_DIR ${UUIDXX_DIR}/cmake)

include(${UUIDXX_CMAKE_DIR}/CPM.cmake)

# Compiler and output configurations.
if(MSVC)
  string (REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  include(${UUIDXX_CMAKE_DIR}/compiler_msvc.cmake)
  foreach(OUTPUTCONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG_TYPE} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG_TYPE}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG_TYPE}/lib)
  endforeach()
else()
  include(${UUIDXX_CMAKE_DIR}/compiler_posix.cmake)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

if(NOT MSVC)
  message(STATUS "BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
endif()

add_subdirectory(third_party)
add_subdirectory(uuidxx)

if(UUIDXX_NOT_SUBPROJECT)
  add_subdirectory(tests)
endif()

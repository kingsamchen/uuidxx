cmake_minimum_required(VERSION 3.19)

# Detect if being bundled via sub-directory.
if(NOT DEFINED PROJECT_NAME)
  set(UUIDXX_NOT_SUBPROJECT ON)
else()
  set(UUIDXX_NOT_SUBPROJECT OFF)
endif()

message(STATUS "uuidxx as root project = ${UUIDXX_NOT_SUBPROJECT}")

project(uuidxx C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(UUIDXX_NOT_SUBPROJECT)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  set(ROOT_DIR ${CMAKE_SOURCE_DIR})
endif()

set(UUIDXX_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(UUIDXX_CMAKE_DIR ${UUIDXX_DIR}/cmake)

include(CTest)
include(${UUIDXX_CMAKE_DIR}/CPM.cmake)

message(STATUS "uuidxx GENERATOR = " ${CMAKE_GENERATOR})

# Output configurations.
get_property(MULTICONF_GENERATOR GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(UUIDXX_NOT_SUBPROJECT)
  if(MULTICONF_GENERATOR)
    foreach(OUTPUTCONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
      string(TOUPPER ${OUTPUTCONFIG_TYPE} OUTPUTCONFIG)
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG_TYPE}/bin)
      set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG_TYPE}/lib)
    endforeach()
  else()
    if(NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE "Release")
    endif()
    message(STATUS "uuidxx BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  endif()
else()
  if(NOT MULTICONF_GENERATOR)
    message(STATUS "uuidxx BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
  endif()
endif()

# Compiler configurations.
if(MSVC)
  include(${UUIDXX_CMAKE_DIR}/compiler_msvc.cmake)
else()
  include(${UUIDXX_CMAKE_DIR}/compiler_posix.cmake)
endif()

include(${UUIDXX_CMAKE_DIR}/clang_tidy.cmake)

add_subdirectory(third_party)
add_subdirectory(uuidxx)

if(UUIDXX_NOT_SUBPROJECT AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

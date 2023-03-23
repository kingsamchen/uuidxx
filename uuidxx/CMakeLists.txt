
find_package(Threads REQUIRED)

add_library(uuidxx)

target_sources(uuidxx
  PRIVATE
    uuidxx.h

    clock_sequence.cpp
    clock_sequence.h
    dce_host_identifier.h
    endian_utils.h
    node_fetcher.cpp
    node_fetcher.h
    rand_generator.h
    uuid.cpp
    uuid.h

  $<$<BOOL:${WIN32}>:
    mac_address_win.cpp
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
    mac_address_posix.cpp
  >
)

target_include_directories(uuidxx
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(uuidxx
  PUBLIC
    Threads::Threads

  PRIVATE
    hash
)

uuidxx_apply_common_compile_options(uuidxx)

if(UUIDXX_ENABLE_CLANG_TIDY)
  uuidxx_apply_clang_tidy(uuidxx)
endif()

if(MSVC)
  if(UUIDXX_USE_MSVC_PARALLEL_BUILD)
    uuidxx_apply_msvc_parallel_build(uuidxx)
  endif()
  if(UUIDXX_USE_MSVC_STATIC_ANALYSIS)
    uuidxx_apply_msvc_static_analysis(uuidxx
      WDL
        /wd6011 # Dereferencing potentially NULL pointer.
    )
  endif()
else()
  if(UUIDXX_USE_SANITIZER)
    uuidxx_apply_sanitizer(uuidxx)
  endif()
endif()

get_target_property(uuidxx_FILES uuidxx SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${uuidxx_FILES})

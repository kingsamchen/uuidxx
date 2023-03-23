
add_library(hash)

target_sources(hash
  PRIVATE
    hash/md5.c
    hash/md5.h
    hash/sha1.c
    hash/sha1.h
)

target_include_directories(hash
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

uuidxx_apply_common_compile_options(hash)

if(MSVC)
  if(UUIDXX_USE_MSVC_PARALLEL_BUILD)
    uuidxx_apply_msvc_parallel_build(hash)
  endif()
else()
  if(UUIDXX_USE_SANITIZER)
    uuidxx_apply_sanitizer(hash)
  endif()
endif()

get_target_property(hash_FILES hash SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${hash_FILES})

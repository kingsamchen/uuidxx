
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

get_target_property(hash_FILES hash SOURCES)
source_group("hash" FILES ${hash_FILES})

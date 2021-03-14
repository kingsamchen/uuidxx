CPMAddPackage("gh:catchorg/Catch2@2.13.4")

add_executable(uuidxx_test)

target_sources(uuidxx_test
  PRIVATE
    main.cpp
)

apply_uuidxx_compile_conf(uuidxx_test)

target_link_libraries(uuidxx_test
  PRIVATE
    uuidxx
    Catch2
)

get_target_property(test_FILES uuidxx_test SOURCES)
source_group("tests" FILES ${test_FILES})

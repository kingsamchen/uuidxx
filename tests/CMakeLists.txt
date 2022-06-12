CPMAddPackage("gh:catchorg/Catch2@2.13.9")

add_executable(uuidxx_test)

target_sources(uuidxx_test
  PRIVATE
    main.cpp
    uuid_test.cpp
)

target_link_libraries(uuidxx_test
  PRIVATE
    uuidxx
    Catch2
)

uuidxx_apply_common_compile_options(uuidxx_test)

if(MSVC)
  get_target_property(test_FILES uuidxx_test SOURCES)
  source_group("tests" FILES ${test_FILES})

  if(UUIDXX_USE_MSVC_PARALLEL_BUILD)
    uuidxx_apply_msvc_parallel_build(uuidxx_test)
  endif()
  if(UUIDXX_USE_MSVC_STATIC_ANALYSIS)
    uuidxx_apply_msvc_static_analysis(uuidxx_test)
  endif()
else()
  if(UUIDXX_USE_SANITIZER)
    uuidxx_apply_sanitizer(uuidxx_test)
  endif()
endif()

add_test(NAME test_uuidxx COMMAND uuidxx_test)

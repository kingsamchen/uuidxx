
add_library(uuidxx)

target_sources(uuidxx
  PRIVATE
    uuid.h
    uuid.cpp
    uuidxx.h

  $<$<BOOL:${WIN32}>:
  >

  $<$<NOT:$<BOOL:${WIN32}>>:
  >
)

target_include_directories(uuidxx
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(uuidxx
)

apply_uuidxx_compile_conf(uuidxx)

get_target_property(uuidxx_FILES uuidxx SOURCES)
source_group("uuidxx" FILES ${uuidxx_FILES})

if(MSVC AND UUIDXX_ENABLE_CODE_ANALYSIS)
  enable_uuidxx_msvc_static_analysis_conf(uuidxx
    WDL
      /wd6011 # Dereferencing potentially NULL pointer.
  )
endif()

set_target_properties(uuidxx PROPERTIES
    VS_GLOBAL_RunCodeAnalysis true

    # Use visual studio core guidelines
    # Tweak as your wish.
    VS_GLOBAL_EnableMicrosoftCodeAnalysis true
    # VS_GLOBAL_CodeAnalysisRuleSet ${CMAKE_CURRENT_SOURCE_DIR}/foo.ruleset
    # VS_GLOBAL_CodeAnalysisRuleSet ${CMAKE_CURRENT_SOURCE_DIR}/foo.ruleset

    # Use clangtidy
    # Tweak as your wish.
    VS_GLOBAL_EnableClangTidyCodeAnalysis true
    # VS_GLOBAL_ClangTidyChecks -checks=-*,modernize-*,-modernize-use-trailing-return-type
)

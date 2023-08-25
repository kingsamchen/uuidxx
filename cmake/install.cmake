
include(CMakePackageConfigHelpers)

set(UUIDXX_INSTALL_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/uuidxx CACHE STRING
  "Installation directory for the library, in relative path")

configure_package_config_file(
  ${UUIDXX_CMAKE_DIR}/uuidxx-config.cmake.in
  ${PROJECT_BINARY_DIR}/uuidxx-config.cmake
  INSTALL_DESTINATION ${UUIDXX_INSTALL_CMAKE_DIR}
)

install(TARGETS hash uuidxx
  EXPORT uuidxx-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT uuidxx-targets
  NAMESPACE uuidxx::
  DESTINATION ${UUIDXX_INSTALL_CMAKE_DIR}
)

if(WIN32)
  set(UUIDXX_HEADER_EXCLUDED "(_linux\\.h)$")
else()
  set(UUIDXX_HEADER_EXCLUDED "(_win\\.h)$")
endif()

# Install headers while keeping folder structure.
install(DIRECTORY
  ${UUIDXX_DIR}/uuidxx/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/uuidxx
  FILES_MATCHING PATTERN "*.h"
  PATTERN REGEX ${UUIDXX_HEADER_EXCLUDED} EXCLUDE
)

install(FILES
  ${PROJECT_BINARY_DIR}/uuidxx-config.cmake
  DESTINATION ${UUIDXX_INSTALL_CMAKE_DIR}
)

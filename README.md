# uuidxx

A modern C++ implementation of Universally Unique Identifiers (UUID) variant as defined in RFC-4122.

This library intends to support the following UUID versions, all are RFC-4122 compliant:

- Version 1, based on timestamp and MAC address as node id
- Version 2, based on DCE security version <sup>[1]</sup>
- Version 3, based on MD5 hashing of a named value
- Version 4, based on random numbers
- Version 5, based on SHA-1 hashing of a named value

1. The provided v2 implementation is included only for completeness, it may not be fully compliant to DCE's security requirements. See comments in the source file.
DO NOT use it in production.

## Basic usage

```cpp
#include <iostream>

#include "uuidxx/uuidxx.h"

int main()
{
    auto u = uuidxx::make_v4();
    std::cout << "Generated v4 UUID " << u.to_string();

    return 0;
}
```

## Adding to you project

### Integrate with Source Repo

uuidxx is sub-project aware, therefore you can

1. Download the uuidxx and copy it into a subdirectory in your CMake project or add uuidxx as a git submodule
2. Then use `add_subdirectory()` to include uuidxx direclty in your CMake project
3. Use `target_link_libraries()` declare dependency of uuidxx.

```cmake
cmake_minimum_required(VERSION 3.16)

project(my_project)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(uuidxx)

add_executable(my_exe source.cpp)

target_link_libraries(my_exe uuidxx)
```

### Integrate with CPM.cmake

[CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) is a setup-free CMake dependency management.

Simply download the cmake file and add to your CMake project, then

```cmake
cmake_minimum_required(VERSION 3.16)

project(my_project)

set(CMAKE_CXX_STANDARD 17)

include(cmake/CPM.cmake)
CPMAddPackage("gh:kingsamchen/uuidxx")

add_executable(my_exe source.cpp)

target_link_libraries(my_exe uuidxx)
```

## Build instructions

Building uuidxx requires:

- C++ 17 compatible compiler <sup>[1]</sup>
- CMake 3.16 or higher
- Ninja build (optional) <sup>[2]</sup>

1. It is possible to back port to C++ 11 & 14
2. Ninja build is used only on *nix platforms, and Makefile would be used if ninja was not found.

### Windows/MSVC

```powershell
$ cd uuidxx
$ ./anvil.ps1 [-build-type <Debug|Release>] [-no-build]
```

If not specified, default build is in `Release` mode; and if `-no-build` is specified, generate Visual Studio solution project only.

### Linux

```shell
$ cd uuidxx
$ ./anvil.sh [--build-type=<Debug|Release>] [--no-build]
```

If not specified, default build is in `Release` mode; and if `--no-build` is specified, generate makefile or build.ninja files only.

You can force generator by:

```shell
$ GENERATOR="Unix Makefiles" ./anvil.sh
```

## License

uuidxx is licensed under the terms of the MIT license. see [LICENSE](https://github.com/kingsamchen/uuidxx/blob/master/LICENSE)

## References

- [RFC-4122](https://tools.ietf.org/html/rfc4122)
- [DCE 1.1 Remote Procedure Call - Universal Unique Identifier](https://pubs.opengroup.org/onlinepubs/9629399/apdxa.htm)
- [DCE 1.1 Authentication and Security Services - Security-Version (Version 2) UUIDs](https://pubs.opengroup.org/onlinepubs/9696989899/chap5.htm#tagcjh_08_02_01_01)
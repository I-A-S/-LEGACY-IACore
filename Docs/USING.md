## 🚀 Using IACore in a New Project

IACore provides a CMake macro `iacore_setup_project()`, which standardizes your build environment. This macro automatically:

* Enforces C++20 standard.
* Sets warning levels (-Wall -Wextra -Wpedantic for Clang/GCC, /W4 for MSVC/Clang-CL).
* Detects the target architecture (x64, ARM64, WASM) and sets internal cache variables.
* Suppresses C++98 compatibility warnings when using Clang on Windows.

Example CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.28)
project(MyGame)

add_subdirectory(external/IACore)

## Apply IACore's standard project configuration
# This applies C++20 and strict warning flags globally to your targets.
iacore_setup_project()

# Define your target(s)
add_executable(MyGame src/main.cpp)

# Link IACore
target_link_libraries(MyGame PRIVATE IACore)
```

cmake_minimum_required(VERSION 3.20)
project(Lox)

add_executable(
  lox
  src/chunk.cpp
  src/compiler.cpp
  src/debug.cpp
  src/main.cpp
  src/scanner.cpp
  src/vm.cpp
)

set_property(TARGET lox PROPERTY CXX_STANDARD 23)
target_compile_options(lox PRIVATE -Wall -Werror -Wextra)
target_include_directories(lox PRIVATE include)

find_package(Catch2 3 REQUIRED)

# add_executable(tests test.cpp)
# target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

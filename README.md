# liquid.cpp

A small, dependency-free C++20 starter project built with CMake.

## Build and run

You need CMake 3.20+, Ninja, and a C++20 compiler (Clang, GCC, or MSVC).

```sh
cmake --preset dev
cmake --build --preset dev
./build/dev/liquid "your name"
```

## Run tests

```sh
ctest --preset dev
```

## Project layout

- `include/liquid/` — public headers
- `src/` — implementation and executable entry point
- `tests/` — tests registered with CTest

The target named `liquid_lib` holds reusable code. The `liquid` executable and
tests link against it. Add new `.cpp` files to `add_library` in `CMakeLists.txt`.

```text
 _ _             _     _                   
| (_) __ _ _   _(_) __| |  ___ _ __  _ __  
| | |/ _` | | | | |/ _` | / __| '_ \| '_ \ 
| | | (_| | |_| | | (_| || (__| |_) | |_) |
|_|_|\__, |\__,_|_|\__,_(_)___| .__/| .__/ 
        |_|                   |_|   |_|    
```

# liquid.cpp

The first milestone is deliberately small: locate the cached LFM2.5-350M
MLX/BF16 model, allocate space for its weights, and print its safetensors index.

A small C++20 inference-engine project built with CMake and nlohmann/json.

All human and LLM contributors should read [CONTRIBUTING.md](CONTRIBUTING.md)
before setting up the development environment or making changes.

## Build and run

You need CMake 3.20+, Ninja, and a C++20 compiler (Clang, GCC, or MSVC).

```sh
cmake --preset dev
cmake --build --preset dev
./build/dev/liquid --default
./build/dev/liquid -hf LiquidAI/LFM2.5-350M-MLX-bf16
```

For the normal development loop:

```sh
./scripts/dev.sh
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

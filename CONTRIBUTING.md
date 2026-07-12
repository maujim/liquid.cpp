# Contributing

This repository is an intentionally small C++ inference-engine project. The
current milestone only discovers the cached LFM2.5-350M MLX/BF16 model,
allocates a buffer for `model.safetensors`, and prints
`model.safetensors.index.json`.

## Development environment

You need:

- CMake 3.20 or newer
- Ninja
- A C++20 compiler (AppleClang, Clang, GCC, or MSVC)
- Git
- Network access on the first CMake configure, so CMake can download the
  pinned `nlohmann/json` dependency
- The Hugging Face snapshot at:

  `~/.cache/huggingface/hub/models--LiquidAI--LFM2.5-350M-MLX-bf16`

The executable currently supports only `LiquidAI/LFM2.5-350M-MLX-bf16` and
expects its `refs/main` file and `snapshots/<revision>/model.safetensors` to be
present. The model is not committed to this repository.

## Build and test

From the repository root:

```sh
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

Run the executable directly with either supported form:

```sh
./build/dev/liquid --default
./build/dev/liquid -hf LiquidAI/LFM2.5-350M-MLX-bf16
```

## Project layout

- `include/liquid/` — public headers
- `src/` — implementation and executable entry point
- `tests/` — tests registered with CTest

The target named `liquid_lib` holds reusable code. The `liquid` executable and
tests link against it. Add new `.cpp` files to `add_library` in `CMakeLists.txt`.

## Contributor workflow

Keep changes small and easy to inspect. Add or update tests when behavior
changes, run the build and CTest suite before committing, and avoid committing
the model, generated build directories, or local cache files. The project uses
the C++ standard library and CMake for its build; do not add a heavyweight
framework without discussing why it is needed.

### For LLM-assisted contributions

An agent should inspect the existing source and run the tests before editing.
It should describe assumptions when model-format details are uncertain, keep
the implementation understandable to a human learning C++, and avoid silently
implementing the inference kernels on the user's behalf. Generated changes
should be reviewed, built, and tested by a human contributor before merging.

#!/usr/bin/env bash

set -u

ROOT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
cd "$ROOT_DIR"

WATCH_PATHS=(
  CMakeLists.txt
  CMakePresets.json
  include
  src
  tests
)

build() {
  printf '\n[%s] Building...\n' "$(date '+%H:%M:%S')"
  cmake --build --preset dev
}

run_binary() {
  printf '[%s] Running...\n' "$(date '+%H:%M:%S')"
  ./build/dev/liquid --default
}

# Configure once so this also works from a clean checkout.
cmake --preset dev || exit $?
if build; then
  run_binary "$@"
fi

if command -v fswatch >/dev/null 2>&1; then
    fswatch -o --event Created --event Updated --event Removed --event Renamed \
    "${WATCH_PATHS[@]}" | while read -r _; do
      if build; then
        run_binary "$@"
      fi
    done
else
  echo "fswatch not found; polling for changes every second."
  previous_hash=''

  while true; do
    current_hash=$(
      find "${WATCH_PATHS[@]}" -type f -print0 2>/dev/null |
        sort -z |
        xargs -0 shasum 2>/dev/null |
        shasum
    )

    if [[ -n "$previous_hash" && "$current_hash" != "$previous_hash" ]]; then
      if build; then
        run_binary "$@"
      fi
    fi

    previous_hash=$current_hash
    sleep 1
  done
fi

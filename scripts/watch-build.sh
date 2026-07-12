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

# Configure once so this also works from a clean checkout.
cmake --preset dev || exit $?
build || true

if command -v fswatch >/dev/null 2>&1; then
  fswatch -o --event Created --event Updated --event Removed --event Renamed \
    "${WATCH_PATHS[@]}" | while read -r _; do
      build || true
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
      build || true
    fi

    previous_hash=$current_hash
    sleep 1
  done
fi

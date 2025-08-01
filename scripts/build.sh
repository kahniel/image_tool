#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

cd "$PROJECT_ROOT"

echo "Building Image Tool..."
mkdir -p build
cd build
cmake ..
make -j$(nproc)
echo "Build complete! Binaries in build/bin/"

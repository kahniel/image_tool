#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

cd "$PROJECT_ROOT"

echo "=== Image Tool Examples ==="
echo "1. Downscaling:"
./build/bin/image_tool resize -i data/ponyo.png -o small_ponyo.jpg -s 4 -m my-down

echo "2. Upscaling with Real-ESRGAN:"
./build/bin/image_tool resize -i small_ponyo.jpg -o processed_ponyo.png -s 4 -m realesrgan

echo "3. comparing images with SSIM:"
./build/bin/image_tool compare -1 data/ponyo.png -2 processed_ponyo.png -m ssim

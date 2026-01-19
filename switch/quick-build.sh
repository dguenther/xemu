#!/bin/bash
# Quick rebuild script for Switch development
# This script builds the Switch NRO package using podman

set -e

echo "=== xemu Switch Quick Build ==="
echo "Starting build at $(date)"

# Run build in container
podman run --rm -v "$(pwd):/src:z" -w /src devkitpro/devkita64 bash -c '
source $DEVKITPRO/switchvars.sh
make -f switch/Makefile.switch -j$(nproc) nro
'

echo "=== Build Complete ==="
echo "Output: dist-switch/xemu.nro"
ls -lh dist-switch/xemu.nro

echo ""
echo "To deploy to Switch:"
echo "  1. Wireless: ./switch/deploy.sh <SWITCH_IP>"
echo "  2. SD Card:  cp dist-switch/xemu.nro /path/to/sd/switch/xemu/"

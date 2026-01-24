#!/bin/bash
# Quick rebuild script for Switch development
# This script builds the Switch NRO package using podman

set -e

echo "=== xemu Switch Quick Build ==="
echo "Starting build at $(date)"

# Run build in container
podman run --rm -t -v "$(pwd):/src:z" -w /src devkitpro/devkita64 bash -c '
source $DEVKITPRO/switchvars.sh
make -f switch/Makefile.switch SWITCH_FULL=1 -j$(nproc) nro
'

echo "=== Build Complete ==="
echo "Output: dist-switch/xemu.nro"

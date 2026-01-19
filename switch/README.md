# xemu Nintendo Switch Port

This directory contains the files needed to build xemu for Nintendo Switch using devkitpro and libnx.

## Overview

This port aims to run xemu (Xbox emulator) on Nintendo Switch homebrew. The Switch's OpenGL 4.3 support via libnx mesa makes this feasible, as xemu's NV2A GPU emulation requires OpenGL 4.0.

## Requirements

### Development Environment

- devkitpro with devkitA64 toolchain
- libnx (Switch homebrew library)
- switch-sdl2
- switch-mesa (provides OpenGL 4.3)
- switch-glad
- switch-zlib
- switch-libpng

### Installation (Ubuntu/Debian)

```bash
# Install devkitpro
wget https://apt.devkitpro.org/install-devkitpro-pacman
chmod +x ./install-devkitpro-pacman
sudo ./install-devkitpro-pacman

# Install Switch development packages
sudo dkp-pacman -S switch-dev switch-sdl2 switch-mesa switch-glad switch-zlib switch-libpng
```

### Using Podman/Docker (Recommended)

```bash
# Pull devkitpro image
podman pull devkitpro/devkita64:latest

# Run build in container (from xemu root directory)
podman run --rm -v "$(pwd):/xemu:Z" -w /xemu devkitpro/devkita64 \
    make -f switch/Makefile.switch

# Or build the Switch abstraction layer files only:
podman run --rm -v "$(pwd):/xemu:Z" -w /xemu devkitpro/devkita64 bash -c '
source $DEVKITPRO/switchvars.sh
ARCH="-march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIC"
CFLAGS="-g -Wall -O2 -ffunction-sections $ARCH"
CFLAGS="$CFLAGS -DCONFIG_SWITCH=1 -D__SWITCH__"
CFLAGS="$CFLAGS -I/xemu/switch -I$DEVKITPRO/libnx/include"
CFLAGS="$CFLAGS -I$DEVKITPRO/portlibs/switch/include/SDL2"
aarch64-none-elf-gcc $CFLAGS -c switch/glib-compat.c -o switch/glib-compat.o
aarch64-none-elf-gcc $CFLAGS -c switch/oslib-switch.c -o switch/oslib-switch.o
aarch64-none-elf-gcc $CFLAGS -c switch/main-switch.c -o switch/main-switch.o
'
```

Note: The `:Z` flag is needed on SELinux systems (Fedora, RHEL, etc.) for proper volume permissions.

## Building

From the xemu root directory:

```bash
# Set devkitpro environment
export DEVKITPRO=/opt/devkitpro

# Build
make -f switch/Makefile.switch

# Build NRO package
make -f switch/Makefile.switch nro
```

The output will be in `dist-switch/`:
- `xemu.elf` - ELF executable
- `xemu.nro` - Nintendo Switch homebrew package

## File Structure

```
switch/
├── Makefile.switch      # Main build file for Switch
├── config-switch.h      # Hardcoded build configuration
├── platform_stubs.h     # Stubs for missing POSIX features
├── glib-compat.h        # GLib compatibility layer header
├── glib-compat.c        # GLib compatibility implementations
├── oslib-switch.c       # OS abstraction layer for Switch
├── main-switch.c        # Switch-specific main entry point
└── README.md            # This file
```

## Switch Directory Structure

On the Switch SD card, xemu expects:

```
sdmc:/switch/xemu/
├── xemu.nro             # The xemu homebrew application
├── bios/
│   ├── mcpx_1.0.bin     # MCPX ROM (required)
│   └── bios.bin         # Xbox BIOS (required)
├── discs/               # ISO/XISO disc images
├── saves/               # Save states
├── data/                # Runtime data
└── xbox_hdd.qcow2       # Xbox HDD image (required)
```

## Current Status

### What Works
- [x] Build system setup
- [x] GLib compatibility layer (2375+ lines)
- [x] Platform abstraction stubs
- [x] Switch platform files compile successfully (verified with podman)
- [x] Full QEMU/xemu core links successfully (15.8 MB ELF)
- [x] NRO package builds (6.1 MB)
- [ ] TCG (dynamic recompilation)
- [ ] NV2A GPU emulation (OpenGL)
- [ ] Audio emulation (SDL)
- [ ] Controller input (Joy-Con/Pro Controller)
- [ ] Game loading

### Known Limitations

1. **Performance**: Xbox emulation is computationally intensive. Expect lower performance than on desktop.

2. **Memory**: Switch has ~4GB RAM in homebrew mode. Xbox requires 64MB + overhead.

3. **No Networking**: Xbox Live and network features are not supported.

4. **Storage**: Games must be stored on SD card. Large ISOs may have slow load times.

## Technical Notes

### Graphics
- Uses OpenGL 4.3 via libnx mesa
- Vulkan renderer is disabled (OpenGL only)
- Resolution may be limited to 720p for performance

### CPU Emulation
- Uses QEMU's TCG (Tiny Code Generator)
- Translates i386 Xbox CPU to ARM64

### Audio
- SDL2 audio backend
- May have latency issues

### Input Mapping
Default controller mapping:
- A/B/X/Y → Xbox face buttons
- L/R/ZL/ZR → Xbox triggers/bumpers
- D-pad → Xbox D-pad
- Sticks → Xbox analog sticks
- + → Start
- - → Back

## Troubleshooting

### Build Errors

**"DEVKITPRO is not set"**
```bash
export DEVKITPRO=/opt/devkitpro
source $DEVKITPRO/switchvars.sh
```

**Missing headers**
Ensure all switch-* packages are installed via dkp-pacman.

### Runtime Issues

**Black screen on launch**
- Check that BIOS files are in the correct location
- Verify OpenGL context creation in log file

**Crashes immediately**
- Check `sdmc:/switch/xemu/xemu.log` for error messages
- Ensure you have enough free memory

**Poor performance**
- Try simpler games first
- Ensure CPU boost mode is enabled (automatic)

## Contributing

When contributing Switch-specific code:

1. Keep platform code isolated in `switch/` directory
2. Use `#ifdef __SWITCH__` for Switch-only code paths
3. Test on actual hardware when possible
4. Document any libnx API usage

## License

This port follows the same license as xemu (GPL-2.0-or-later).

## Credits

- xemu team for the Xbox emulator
- QEMU team for the emulation infrastructure
- devkitpro team for Switch development tools
- libnx team for Switch homebrew libraries
- switchbrew community for documentation

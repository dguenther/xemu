# Runtime Fixes - Switch Hardware Testing

Consolidated log of all runtime errors encountered during Switch hardware testing and their fixes.

---

## Fix #1: BIOS Loading - `qemu_find_file`

**Error:** `Failed to load BIOS '(null)'`

**Cause:** `qemu_find_file()` was stubbed (returned 0/NULL) instead of using real implementation.

**Fix:** Removed stub. Real implementation from `system/datadir.c` now compiles and properly resolves file paths.

**Technical:** The real `qemu_find_file()` checks if file exists with `qemu_access()`, returns `g_strdup(name)` if found, searches data directories if not, returns NULL if missing.

---

## Fix #2: Migration Flag - `vmstate_check_only_migratable`

**Error:** `xemu: Device xbox-pcihost is not migratable, but --only-migratable was specified`

**Cause:** Stub returned 0 (false), triggering migration check failure in `hw/core/qdev.c:445`.

**Fix:** Added proper stub in `switch/qemu-stubs.c` returning `true` to allow all devices.

**Rationale:** Switch doesn't support VM migration. Returning `true` bypasses migration checks for all devices.

---

## Fix #3: Unknown Type 'resettable-container'

**Error:** `xemu: unknown type 'resettable-container'`

**Cause:** QOM type `TYPE_RESETTABLE_CONTAINER` not registered because `hw/core/resetcontainer.c` wasn't compiled.

**Fix:** Added `hw/core/resetcontainer.c` to `switch/Makefile.switch`.

**Technical:** File contains `OBJECT_DEFINE_SIMPLE_TYPE_WITH_INTERFACES` macro that registers QOM type at module init. Type is used by `hw/core/reset.c`.

---

## Fix #4: Default BIOS Paths

**Error:** `Failed to load BIOS '(null)'` (persisted after Fix #1)

**Cause:** `g_config.sys.files.flashrom_path` was empty because Switch settings don't have defaults on first run.

**Fix:** Added default path configuration in `switch/main-switch.c` after `xemu_settings_load()`:
- BIOS: `sdmc:/switch/xemu/bios/bios.bin`
- MCPX ROM: `sdmc:/switch/xemu/bios/mcpx_1.0.bin`
- HDD: `sdmc:/switch/xemu/bios/xbox_hdd.qcow2`
- EEPROM: `sdmc:/switch/xemu/bios/eeprom.bin`

**Technical:** Without paths, `system/vl.c:2953` doesn't add `-bios` parameter to command line, resulting in `ms->firmware = NULL`.

---

## Fix #5: Welcome Screen Blocking Boot

**Error:** `-bios` parameter missing from command line despite default paths being set.

**Cause:** `g_config.general.show_welcome` defaults to `true` in `config_spec.yml`. When true, `system/vl.c:2942` skips adding `-bios` to allow user to configure paths via welcome screen UI.

**Fix:** Added `g_config.general.show_welcome = false;` in `switch/main-switch.c` after settings load.

**Rationale:** Switch has no welcome screen UI and should boot immediately with default paths.

---

## Fix #6: i8259 PIC Initialization

**Error:** NULL pointer dereference in `pc_i8259_create()` at offset `0x87824/0x87828`.

**Cause:** `i8259_init()` and `x86_allocate_cpu_irq()` were stubbed (returned NULL). `pc_i8259_create()` in `hw/i386/pc.c:1271` tried to dereference NULL array.

**Fix:** Added real implementations to build:
- `hw/i386/x86-cpu.c` - x86 CPU and interrupt handling
- `hw/intc/i8259.c` - i8259 PIC implementation
- `hw/intc/i8259_common.c` - i8259 common code

**Additional stubs needed:**
- Removed `cpu_get_tsc` stub (now using real implementation)
- Added trace DSTATE variables: `_TRACE_PIC_UPDATE_IRQ_DSTATE`, `_TRACE_PIC_IOPORT_WRITE_DSTATE`, `_TRACE_PIC_SET_IRQ_DSTATE`, `_TRACE_PIC_IOPORT_READ_DSTATE`, `_TRACE_X86_PIC_INTERRUPT_DSTATE`
- Added qdev GPIO stubs: `qdev_init_gpio_out()`, `qdev_init_gpio_in()`, `qdev_get_gpio_in()`
- Added APIC stubs: `cpu_is_apic_enabled()`, `apic_accept_pic_intr()`, `apic_deliver_pic_intr()`

**Technical:** The i8259 PIC (Programmable Interrupt Controller) manages hardware interrupts (IRQs) and routes them to CPU. Required for keyboard, mouse, disk, and other device interrupts. Xbox uses this for legacy interrupt handling.

---

## Fix #7: Missing intctrl Interface

**Error:** `xemu: missing interface 'intctrl' for object 'isa-device'`

**Cause:** `hw/intc/intc.c` not compiled. This file registers the `intctrl` interface (TYPE_INTERRUPT_STATS_PROVIDER) with QEMU's type system via `type_init()` macro.

**Fix:** Added `hw/intc/intc.c` to `switch/Makefile.switch` after other intc files.

**Technical:** QEMU's QOM uses interfaces like mix-ins. When i8259 and ISA devices implement the `intctrl` interface, it must be registered at startup. Error occurs at `qom/object.c:390` when creating objects with unregistered interfaces.

---

## Fix #8: Missing isa-pit Type

**Error:** `xemu: unknown type 'isa-pit'`

**Cause:** i8254 PIT (Programmable Interval Timer) type not registered because `hw/timer/i8254.c` and `hw/timer/i8254_common.c` not compiled.

**Fix:** Added to `switch/Makefile.switch`:
- `hw/timer/i8254.c` - i8254 PIT implementation
- `hw/timer/i8254_common.c` - i8254 common code

**Technical:** The i8254 PIT is the system timer that generates periodic interrupts for timekeeping and scheduling. Xbox uses this for system timing. Type name `"isa-pit"` is defined as `TYPE_I8254` in `include/hw/timer/i8254.h:53`.

---

## Fix #9: Missing PC Speaker 'pit' Property

**Error:** `xemu: Property 'isa-pcspk.pit' not found`

**Cause:** PC speaker stub in `switch/isa-pcspk-stub.c` only defined "audiodev" property but was missing the "pit" link property that connects it to the PIT timer.

**Fix:** Updated `switch/isa-pcspk-stub.c`:
- Added `void *pit` field to `PCSpkStubState` structure
- Added `pcspk_stub_instance_init()` function to create "pit" link property
- Hooked up instance_init in TypeInfo
- Added includes for `hw/qdev-properties.h` and `hw/timer/i8254.h`

**Technical:** PC speaker device needs a link to PIT timer channel 2 for generating tones. Real implementation uses `object_property_add_link(obj, "pit", TYPE_PIT_COMMON, ...)` in instance_init. Stub now provides same property interface even though PC speaker functionality isn't implemented.

---

## Fix #10: Missing piix3-ide Type

**Error:** `xemu: unknown type 'piix3-ide'`

**Cause:** PIIX3 IDE controller type not registered because IDE subsystem files not compiled.

**Fix:** Added IDE subsystem to `switch/Makefile.switch`:
- `hw/ide/core.c` - IDE core functionality
- `hw/ide/atapi.c` - ATAPI support (CD-ROM)
- `hw/ide/pci.c` - PCI IDE support
- `hw/ide/piix.c` - PIIX3 IDE controller
- `hw/ide/ide-bus.c` - IDE bus implementation
- `hw/ide/ide-dev.c` - IDE device types
- `hw/ide/ioport.c` - IDE I/O port handling

**Technical:** Xbox uses a PIIX3 IDE controller (PCI function 9.0) for HDD and DVD drive access. Type name `"piix3-ide"` is defined as `TYPE_PIIX3_IDE`. IDE subsystem provides ATA/ATAPI disk and CD-ROM emulation required for Xbox storage.

**Additional stubs required:** ~40+ IDE/Block/AIO stubs added to `qemu-stubs.c`:
- Block accounting: `blk_get_stats`, `block_acct_*`
- Block AIO: `blk_aio_*`, `qemu_aio_unref`, `aio_bh_new_full`
- Block operations: `blk_get_geometry`, `blk_attach_dev`, `blk_drain`, etc.
- Block config: `blkconf_*`, `check_boot_index`, `add_boot_device_*`
- CD-ROM: `cdrom_read_toc`, `cdrom_read_toc_raw`
- IDE trace variables: ~25 `_TRACE_IDE_*_DSTATE` and `_TRACE_BMDMA_*_DSTATE`
- DMA trace variables: `_TRACE_DMA_*_DSTATE`
- QDev properties: `qdev_prop_drive`, `qdev_prop_blocksize`, `qdev_prop_blockdev_on_error`, `qdev_prop_bios_chs_trans`
- Misc: `qemu_hexdump_line`, `qemu_hw_version`, `vmstate_info_buffer`, `replay_bh_schedule_event`

---

## Fix #11: PropertyInfo NULL Pointer Dereference

**Error:** Exception 0x101, NULL pointer dereference at offset 0x10 (PC crash)

**Cause:** PropertyInfo stub structures only had `name`, `get`, and `set` fields. Missing the `create` function pointer caused NULL dereference when IDE device code tried to create properties.

**Fix:** Added `stub_prop_create()` function to PropertyInfo stubs:
- Created static `stub_object_property` to return from create function
- Added `create` function pointer to all PropertyInfo stubs
- Returns dummy ObjectProperty instead of NULL to prevent crashes

**Technical:** PropertyInfo.create() is called during device initialization to create ObjectProperty instances. Initially tried returning a static dummy ObjectProperty, but that caused crashes at offset 0x10 when code tried to access uninitialized fields. Fixed by properly calling `object_class_property_add()` which creates and registers a real ObjectProperty with the QOM system.

**Additional fixes in this round:**
- Moved `#include "hw/qdev-properties.h"` to file header
- Removed duplicate struct definitions (Property, PropertyInfo now from header)
- Fixed function signatures: `void*` → `DeviceState*` for type safety
- Removed conflicting SWITCH_STUB_INT macros for header-declared functions
- Added qdev function stubs: `qdev_prop_set_chr`, `qdev_connect_gpio_out`, `qdev_finalize_clocklist`, `qdev_fw_name`, `qdev_get_hotplug_handler`

**Follow-up fix (same error repeated):** Changed `stub_prop_create()` to call `object_class_property_add()` instead of returning a static dummy. This properly creates ObjectProperty instances registered with QOM, avoiding NULL/uninitialized field access.

**Second follow-up (same error again):** Used `addr2line` to pinpoint crash:
- PC offset `0x3b4bc` → `g_free()` in `switch/glib-compat.h:295`
- LR offset `0x3b498` → `object_property_set_description()` in `qom/object.c:2855`
- Root cause: After `stub_prop_create()` creates property, QEMU calls `object_class_property_set_description()` which tries to `g_free(op->description)`. The ObjectProperty's description field contained garbage, causing crash.
- Fix: Added `.description` field to all PropertyInfo stubs so QEMU has a valid string to set (or NULL if description is unset)

---

## Fix #12: PropertyInfo set_default_value NULL Pointer Crash

**Error:** Exception 0x100 (data abort), PC=0x0 (NULL pointer dereference)

**Debug process:**
```
LR offset: 0x7d894 → qdev_class_add_property (hw/core/qdev-properties.c:1018)
Disassembly at 0x7d894: blr x2 (indirect call through register x2)
```

**Cause:** At `qdev_class_add_property:1014`, when `prop->set_default` is true, code calls `prop->info->set_default_value(op, prop)`. PropertyInfo stub structures in `switch/qemu-stubs/qdev.c` were missing the `set_default_value` function pointer, leaving it NULL. When this function was called, it caused a jump to address 0x0.

**Fix:** Added to `switch/qemu-stubs/qdev.c`:
1. Created `stub_prop_set_default_value()` no-op function
2. Added `.set_default_value = stub_prop_set_default_value` to:
   - `qdev_prop_blockdev_on_error_stub`
   - `qdev_prop_bios_chs_trans_stub`  
   - `qdev_prop_drive_stub`
   - `qdev_prop_blocksize_stub`
   - `qdev_prop_chr` (weak symbol)
   - `qdev_prop_macaddr` (weak symbol)
   - `qdev_prop_netdev` (weak symbol)

**Technical:** In QEMU's property system, when a Property has `.set_default = true`, `qdev_class_add_property()` calls `prop->info->set_default_value()` to initialize default values. The PropertyInfo struct (in `include/hw/qdev-properties.h`) defines this as a function pointer at offset 40 (5 pointers × 8 bytes). Without this field set, the call dereferences NULL and crashes.

---

## Summary

| Fix | Type | Files Modified | Description |
|-----|------|----------------|-------------|
| #1 | Stub→Real | `qemu-stubs.c` | Use real `qemu_find_file` implementation |
| #2 | Stub | `qemu-stubs.c` | Return `true` for migration check |
| #3 | Build | `Makefile.switch` | Add `resetcontainer.c` to register QOM type |
| #4 | Config | `main-switch.c` | Set default BIOS/ROM/HDD/EEPROM paths |
| #5 | Config | `main-switch.c` | Disable welcome screen on Switch |
| #6 | Build+Stubs | `Makefile.switch`, `qemu-stubs.c` | Add i8259 PIC sources + GPIO/APIC stubs |
| #7 | Build | `Makefile.switch` | Add `intc.c` to register intctrl interface |
| #8 | Build | `Makefile.switch` | Add i8254 PIT (timer) sources |
| #9 | Stub | `isa-pcspk-stub.c` | Add 'pit' link property to PC speaker stub |
| #10 | Build+Stubs | `Makefile.switch`, `qemu-stubs.c` | Add IDE subsystem (7 files) + ~40 IDE/Block stubs |
| #11 | Stub | `qemu-stubs.c` | Add PropertyInfo create() function + qdev stubs |
| #12 | Stub | `qemu-stubs/qdev.c` | Add PropertyInfo set_default_value() to prevent NULL crash |

**Total source files added:** 14 (resetcontainer, i8259×2, x86-cpu, intc, i8254×2, ide×7)
**Total stub functions added:** ~70+ (IDE/Block/AIO + qdev + PropertyInfo)
**Total code added:** ~450 lines

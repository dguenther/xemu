# Switch Build Missing References - Priority Fix List

**Total Missing: 179 unique references**

---

## Quick Priority Summary

| Priority | Count | Effort | Description |
|----------|-------|--------|-------------|
| **P1 - Critical** | 35 | 2-3 hours | Must implement to boot |
| **P2 - High** | 40 | 2-3 hours | Full functionality |
| **P3 - Medium** | 50 | 2 hours | Can stub initially |
| **P4 - Low** | 54 | 1 hour | Safe to stub |

---

## PRIORITY 1 - CRITICAL (35 refs)

### System Core (6)
```
qemu_init_cpu_list
qemu_init_main_loop_lock
qemu_mutex_lock_main_loop
main_loop_wait
finalize_target_page_bits
set_preferred_target_page_bits
```

### Accelerator - TCG (5)
```
accel_find
accel_init_machine
accel_init_interfaces
accel_setup_post
current_accel_name
```

### Display (6)
```
init_displaystate
qemu_console_early_init
qemu_display_early_init
qemu_display_init
qemu_display_find_default
qemu_display_get_vc
```

### Block/Drive (5)
```
drive_add
drive_new
drive_get_by_index
bdrv_first
bdrv_next
```

### xemu Specific (3)
```
xbox_eeprom_generate
xemu_net_enable
xemu_net_disable
```

### Config Options (5)
```
qemu_drive_opts
qemu_common_drive_opts
qemu_legacy_drive_opts
qemu_device_opts
qemu_global_opts
```

### Devices (2)
```
qdev_device_add
qdev_device_help
```

### System Variables (3)
```
arch_type
shutdown_action
panic_action
```

---

## PRIORITY 2 - HIGH (40 refs)

### QAPI Visitors (7)
```
visit_type_ObjectOptions
visit_type_DisplayOptions
visit_type_DisplayOptions_members
ObjectType_lookup
DisplayType_lookup
qapi_free_ObjectOptions
qapi_free_DisplayOptions
```

### Monitor (4)
```
monitor_init_globals
monitor_init_opts
monitor_cleanup
monitor_vprintf
```

### Character Devices (2)
```
qemu_chr_new_mux_mon
qemu_chr_cleanup
```

### Firmware/ROM (6)
```
fw_cfg_add_from_generator
fw_cfg_set_order_override
fw_cfg_reset_order_override
rom_check_and_register_reset
rom_set_order_override
rom_reset_order_override
```

### Graphics (4)
```
graphic_width
graphic_height
graphic_depth
qemu_display_help
```

### Migration Infrastructure (4)
```
precopy_infrastructure_init
postcopy_infrastructure_init
migration_object_init
migration_shutdown
```

### System Init (6)
```
qemu_init_arch_modules
qemu_finish_async_prealloc_mem
register_global_state
qemu_add_data_dir
qemu_list_data_dirs
qemu_add_default_firmwarepath
```

### CPU & Config (7)
```
parse_cpu_option
list_cpus
qemu_get_pid_name
configure_rtc
qemu_set_hw_version
socket_init
qemu_global_option
```

---

## PRIORITY 3 - MEDIUM (50 refs)

### Replay System - All Stubs (15)
```
replay_add_blocker
replay_audio_in
replay_audio_out
replay_checkpoint
replay_configure
replay_finish
replay_mutex_lock
replay_mutex_unlock
replay_start
replay_vmstate_init
```

### Audio - Can Defer (6)
```
AudiodevDriver_lookup
select_soundhw
show_valid_soundhw
soundhw_init
qapi_free_Audiodev
visit_type_Audiodev
```

### Network - Can Disable (9)
```
net_init_clients
net_check_clients
net_cleanup
net_client_parse
netdev_is_modern
netdev_parse_modern
qemu_netdev_opts
qemu_net_opts
qemu_nic_opts
```

### QMP Commands (5)
```
qmp_blockdev_add
qmp_cont
qmp_device_add
qmp_marshal_set_action
qmp_migrate_incoming
```

### QAPI Events (6)
```
qapi_event_send_powerdown
qapi_event_send_reset
qapi_event_send_shutdown
qapi_event_send_stop
qapi_event_send_suspend
qapi_event_send_wakeup
```

### Block Cleanup (4)
```
blk_exp_close_all
blk_remove_all_bs
blockdev_close_all_bdrv_states
drive_check_orphaned
```

### Timers (3)
```
timer_mod_anticipate_ns
qemu_clock_enable
icount_configure
```

### NUMA (4)
```
numa_complete_configuration
numa_uses_legacy_mem
parse_numa_opts
qemu_numa_opts
```

### Memory Backend (4)
```
host_memory_backend_get_memory
host_memory_backend_is_mapped
host_memory_backend_set_mapped
vmstate_register_ram_global
```

### Job System (2)
```
job_cancel_sync_all
job_next
```

### Semihosting (6)
```
semihosting_enabled
semihosting_get_argc
semihosting_arg_fallback
qemu_semihosting_enable
qemu_semihosting_config_options
qemu_semihosting_chardev_init
```

---

## PRIORITY 4 - LOW (54 refs)

### Trace System - All Stubs (11)
```
_TRACE_AUDIO_TIMER_DELAYED_DSTATE
_TRACE_AUDIO_TIMER_START_DSTATE
_TRACE_AUDIO_TIMER_STOP_DSTATE
_TRACE_RESETTABLE_RESET_DSTATE
_TRACE_VM_STATE_NOTIFY_DSTATE
_TRACE_VM_STOP_FLUSH_ALL_DSTATE
trace_enable_events
trace_init_backends
trace_init_file
trace_opt_parse
use_rt_clock
```

### Debug/Testing (4)
```
gdb_exit
gdbserver_start
qtest_driver
qtest_server_init
```

### OS-Specific - Not Supported (5)
```
os_set_chroot
os_set_daemonize
os_set_proc_name
os_set_runas
os_setup_limits
```

### Spice - Not Used (2)
```
qemu_spice
using_spice
```

### Character Devices Advanced (3)
```
qemu_chr_new_from_opts
qemu_chr_parse_compat
qemu_chardev_opts
```

### Snapshot/Save State (3)
```
load_snapshot
load_snapshot_resume
dump_vmstate_json_to_file
```

### Boot Order (2)
```
qemu_boot_set
restore_boot_order
```

### USB (1)
```
usbdevice_create
```

### ACPI (2)
```
visit_type_AcpiTableOptions
qapi_free_AcpiTableOptions
```

### Compat Policy (3)
```
visit_type_CompatPolicy
visit_type_CompatPolicy_members
qapi_free_CompatPolicy
```

### Monitor Advanced (1)
```
monitor_fdset_add_fd
```

### Misc (18)
```
override_max_devs
resume_mux_open
suspend_mux_open
smbios_entry_add
term_escape_char
qemu_mon_opts
qemu_trace_opts
qemu_legacy_drive_opts
qemu_semihosting_config_opts
```

---

## Implementation Phases

### Phase 1: Critical Path (~2-3 hours)
**Goal:** Link successfully, potentially boot to black screen

1. Add P1 system core stubs
2. Add P1 accelerator stubs
3. Add P1 display stubs
4. Add P1 block/drive stubs
5. Find real xemu implementations
6. Create QemuOpts structures
7. Add system variables

**Expected Result:** Binary links and starts

---

### Phase 2: Full Init (~2-3 hours)
**Goal:** Complete initialization, handle config

1. Add QAPI visitor stubs
2. Add monitor infrastructure
3. Add firmware/ROM stubs
4. Add migration stubs
5. Complete system init

**Expected Result:** Initialization completes

---

### Phase 3: Subsystem Cleanup (~2 hours)
**Goal:** Disable/stub non-critical systems

1. Stub replay system (all no-ops)
2. Disable network
3. Silent audio
4. Stub QMP commands
5. Stub timers
6. Stub NUMA
7. Stub memory backend

**Expected Result:** Clean boot path

---

### Phase 4: Final Stubs (~1 hour)
**Goal:** Complete remaining stubs

1. Trace system (all no-ops)
2. Debug/test infrastructure
3. OS-specific (not supported)
4. Remaining misc functions

**Expected Result:** Full link success

---

## Quick Reference: Where to Find Real Implementations

### Likely in xemu source (need to compile):
- `xbox_eeprom_generate` → `hw/xbox/smbus_xbox_smc.c` or similar
- `xemu_net_enable` → `system/vl.c` or `hw/xbox/`
- `xemu_net_disable` → `system/vl.c` or `hw/xbox/`

### Likely in QEMU source (need to compile):
- Many QemuOpts → `system/vl.c` or various `*-opts.c` files
- `arch_type` → `system/vl.c` (global variable)
- Accelerator functions → `accel/accel-system.c`, `accel/tcg/`
- Display functions → `ui/console.c`
- Block functions → `block.c`, `blockdev.c`
- Monitor functions → `monitor/monitor.c`

### Likely need real sources added to Makefile:
- `system/cpus.c` - CPU management
- `accel/accel-system.c` - Accelerator management
- `accel/tcg/tcg-accel-ops.c` - TCG operations
- `ui/console.c` - Display console
- `blockdev.c` - Block device management
- Various `hw/*/meson.build` targets

---

## Estimated Total Time
- **Phase 1 (Critical):** 2-3 hours
- **Phase 2 (High):** 2-3 hours
- **Phase 3 (Medium):** 2 hours
- **Phase 4 (Low):** 1 hour

**Total: 7-9 hours to full link**

Most can be simple stubs. Real implementations needed only for critical path.

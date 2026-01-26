# Switch Full Build - Missing References Analysis

**Total Unique Missing References:** 179

---

## Categorized Missing References

### 1. Trace System (11 references)
**TRACE_*_DSTATE Variables:**
- `_TRACE_AUDIO_TIMER_DELAYED_DSTATE`
- `_TRACE_AUDIO_TIMER_START_DSTATE`
- `_TRACE_AUDIO_TIMER_STOP_DSTATE`
- `_TRACE_RESETTABLE_RESET_DSTATE`
- `_TRACE_VM_STATE_NOTIFY_DSTATE`
- `_TRACE_VM_STOP_FLUSH_ALL_DSTATE`

**Trace Functions:**
- `trace_enable_events`
- `trace_init_backends`
- `trace_init_file`
- `trace_opt_parse`
- `use_rt_clock`

---

### 2. QAPI/QMP Visitor & Events (23 references)
**QAPI Visitor Functions:**
- `visit_type_AcpiTableOptions`
- `visit_type_Audiodev`
- `visit_type_CompatPolicy`
- `visit_type_CompatPolicy_members`
- `visit_type_DisplayOptions`
- `visit_type_DisplayOptions_members`
- `visit_type_ObjectOptions`

**QAPI Free Functions:**
- `qapi_free_AcpiTableOptions`
- `qapi_free_Audiodev`
- `qapi_free_CompatPolicy`
- `qapi_free_DisplayOptions`
- `qapi_free_ObjectOptions`

**QAPI Lookup Functions:**
- `AudiodevDriver_lookup`
- `DisplayType_lookup`
- `ObjectType_lookup`

**QAPI Event Send Functions:**
- `qapi_event_send_powerdown`
- `qapi_event_send_reset`
- `qapi_event_send_shutdown`
- `qapi_event_send_stop`
- `qapi_event_send_suspend`
- `qapi_event_send_wakeup`

**QMP Commands:**
- `qmp_blockdev_add`
- `qmp_cont`
- `qmp_device_add`
- `qmp_marshal_set_action`
- `qmp_migrate_incoming`

---

### 3. Accelerator (TCG) (4 references)
- `accel_find`
- `accel_init_interfaces`
- `accel_init_machine`
- `accel_setup_post`
- `current_accel_name`

---

### 4. Block/Drive System (11 references)
- `bdrv_first`
- `bdrv_next`
- `blk_exp_close_all`
- `blk_remove_all_bs`
- `blockdev_close_all_bdrv_states`
- `drive_add`
- `drive_check_orphaned`
- `drive_get_by_index`
- `drive_new`
- `override_max_devs`

---

### 5. Display/Graphics (11 references)
- `graphic_depth`
- `graphic_height`
- `graphic_width`
- `init_displaystate`
- `qemu_console_early_init`
- `qemu_display_early_init`
- `qemu_display_find_default`
- `qemu_display_get_vc`
- `qemu_display_help`
- `qemu_display_init`

---

### 6. Network (8 references)
- `net_check_clients`
- `net_cleanup`
- `net_client_parse`
- `net_init_clients`
- `netdev_is_modern`
- `netdev_parse_modern`

---

### 7. Monitor/Debug (7 references)
- `gdb_exit`
- `gdbserver_start`
- `monitor_cleanup`
- `monitor_fdset_add_fd`
- `monitor_init_globals`
- `monitor_init_opts`
- `monitor_vprintf`

---

### 8. Audio (4 references)
- `select_soundhw`
- `show_valid_soundhw`
- `soundhw_init`

---

### 9. Character Devices (4 references)
- `qemu_chr_cleanup`
- `qemu_chr_new_from_opts`
- `qemu_chr_new_mux_mon`
- `qemu_chr_parse_compat`

---

### 10. Migration/Replay (15 references)
- `migration_object_init`
- `migration_shutdown`
- `postcopy_infrastructure_init`
- `precopy_infrastructure_init`
- `replay_add_blocker`
- `replay_audio_in`
- `replay_audio_out`
- `replay_checkpoint`
- `replay_configure`
- `replay_finish`
- `replay_mutex_lock`
- `replay_mutex_unlock`
- `replay_start`
- `replay_vmstate_init`

---

### 11. Config Options (QemuOpts Variables) (14 references)
- `qemu_chardev_opts`
- `qemu_common_drive_opts`
- `qemu_device_opts`
- `qemu_drive_opts`
- `qemu_global_opts`
- `qemu_legacy_drive_opts`
- `qemu_mon_opts`
- `qemu_netdev_opts`
- `qemu_net_opts`
- `qemu_nic_opts`
- `qemu_numa_opts`
- `qemu_semihosting_config_opts`
- `qemu_spice`
- `qemu_trace_opts`

---

### 12. Memory Backend (4 references)
- `host_memory_backend_get_memory`
- `host_memory_backend_is_mapped`
- `host_memory_backend_set_mapped`
- `vmstate_register_ram_global`

---

### 13. NUMA (2 references)
- `numa_complete_configuration`
- `numa_uses_legacy_mem`

---

### 14. OS/Platform (6 references)
- `os_set_chroot`
- `os_set_daemonize`
- `os_set_proc_name`
- `os_set_runas`
- `os_setup_limits`
- `socket_init`

---

### 15. System Initialization (9 references)
- `finalize_target_page_bits`
- `qemu_add_data_dir`
- `qemu_add_default_firmwarepath`
- `qemu_finish_async_prealloc_mem`
- `qemu_init_arch_modules`
- `qemu_init_cpu_list`
- `qemu_init_main_loop_lock`
- `qemu_mutex_lock_main_loop`
- `set_preferred_target_page_bits`

---

### 16. Main Loop & Timers (5 references)
- `icount_configure`
- `main_loop_wait`
- `qemu_clock_enable`
- `timer_mod_anticipate_ns`

---

### 17. Job System (2 references)
- `job_cancel_sync_all`
- `job_next`

---

### 18. xemu Specific (3 references)
- `xbox_eeprom_generate`
- `xemu_net_disable`
- `xemu_net_enable`

---

### 19. Firmware/ROM (6 references)
- `fw_cfg_add_from_generator`
- `fw_cfg_reset_order_override`
- `fw_cfg_set_order_override`
- `rom_check_and_register_reset`
- `rom_reset_order_override`
- `rom_set_order_override`

---

### 20. Device Management (2 references)
- `qdev_device_add`
- `qdev_device_help`

---

### 21. Boot/Reset (4 references)
- `load_snapshot`
- `load_snapshot_resume`
- `qemu_boot_set`
- `restore_boot_order`

---

### 22. USB (1 reference)
- `usbdevice_create`

---

### 23. Semihosting (4 references)
- `qemu_semihosting_chardev_init`
- `qemu_semihosting_config_options`
- `qemu_semihosting_enable`
- `semihosting_arg_fallback`
- `semihosting_enabled`
- `semihosting_get_argc`

---

### 24. Misc System (13 references)
- `arch_type`
- `configure_rtc`
- `dump_vmstate_json_to_file`
- `list_cpus`
- `panic_action`
- `parse_cpu_option`
- `parse_numa_opts`
- `qemu_get_pid_name`
- `qemu_global_option`
- `qemu_list_data_dirs`
- `qemu_set_hw_version`
- `register_global_state`
- `resume_mux_open`
- `shutdown_action`
- `smbios_entry_add`
- `suspend_mux_open`
- `term_escape_char`
- `qtest_driver`
- `qtest_server_init`
- `using_spice`

---

## Priority Ranking for Implementation

### PRIORITY 1 - CRITICAL (Must implement to boot) - ~35 references

**System Core:**
1. `qemu_init_cpu_list` - CPU initialization
2. `qemu_init_main_loop_lock` - Main loop lock
3. `qemu_mutex_lock_main_loop` - Main loop mutex
4. `main_loop_wait` - Main event loop
5. `finalize_target_page_bits` - Memory setup
6. `set_preferred_target_page_bits` - Memory setup

**Accelerator (TCG required for emulation):**
7. `accel_find` - Find TCG accelerator
8. `accel_init_machine` - Initialize TCG
9. `accel_init_interfaces` - TCG interfaces
10. `accel_setup_post` - Post-init TCG
11. `current_accel_name` - Get accelerator name

**Display (needed for rendering):**
12. `init_displaystate` - Display init
13. `qemu_console_early_init` - Console early init
14. `qemu_display_early_init` - Display early init
15. `qemu_display_init` - Display full init
16. `qemu_display_find_default` - Find display backend
17. `qemu_display_get_vc` - Get virtual console

**Block/Drive (needed for HDD/DVD):**
18. `drive_add` - Add drive
19. `drive_new` - Create drive
20. `drive_get_by_index` - Get drive by index
21. `bdrv_first` - First block device
22. `bdrv_next` - Next block device

**xemu Specific:**
23. `xbox_eeprom_generate` - Generate Xbox EEPROM
24. `xemu_net_enable` - Enable xemu networking
25. `xemu_net_disable` - Disable xemu networking

**Config Options (needed by startup):**
26. `qemu_drive_opts` - Drive options
27. `qemu_common_drive_opts` - Common drive options
28. `qemu_legacy_drive_opts` - Legacy drive options
29. `qemu_device_opts` - Device options
30. `qemu_global_opts` - Global options

**Devices:**
31. `qdev_device_add` - Add device
32. `qdev_device_help` - Device help

**System Variables:**
33. `arch_type` - Architecture type variable
34. `shutdown_action` - Shutdown action variable
35. `panic_action` - Panic action variable

---

### PRIORITY 2 - HIGH (Needed for full functionality) - ~40 references

**QAPI/QMP Visitors (needed for config parsing):**
1. `visit_type_ObjectOptions`
2. `visit_type_DisplayOptions`
3. `visit_type_DisplayOptions_members`
4. `ObjectType_lookup`
5. `DisplayType_lookup`

**QAPI Free Functions:**
6. `qapi_free_ObjectOptions`
7. `qapi_free_DisplayOptions`

**Monitor (for debugging):**
8. `monitor_init_globals`
9. `monitor_init_opts`
10. `monitor_cleanup`
11. `monitor_vprintf`

**Character Devices:**
12. `qemu_chr_new_mux_mon`
13. `qemu_chr_cleanup`

**Firmware/ROM:**
14. `fw_cfg_add_from_generator`
15. `fw_cfg_set_order_override`
16. `fw_cfg_reset_order_override`
17. `rom_check_and_register_reset`
18. `rom_set_order_override`
19. `rom_reset_order_override`

**Graphics:**
20. `graphic_width`
21. `graphic_height`
22. `graphic_depth`
23. `qemu_display_help`

**Migration Infrastructure:**
24. `precopy_infrastructure_init`
25. `postcopy_infrastructure_init`
26. `migration_object_init`
27. `migration_shutdown`

**System Init:**
28. `qemu_init_arch_modules`
29. `qemu_finish_async_prealloc_mem`
30. `register_global_state`

**Config:**
31. `qemu_add_data_dir`
32. `qemu_list_data_dirs`
33. `qemu_add_default_firmwarepath`
34. `qemu_global_option`

**CPU:**
35. `parse_cpu_option`
36. `list_cpus`
37. `qemu_get_pid_name`

**Misc:**
38. `configure_rtc`
39. `qemu_set_hw_version`
40. `socket_init`

---

### PRIORITY 3 - MEDIUM (Can be stubbed initially) - ~50 references

**Replay System (can stub - not needed for initial boot):**
1-14. All `replay_*` functions (15 total)

**Audio (can defer):**
1. `AudiodevDriver_lookup`
2. `select_soundhw`
3. `show_valid_soundhw`
4. `soundhw_init`
5. `qapi_free_Audiodev`
6. `visit_type_Audiodev`

**Audio Config:**
7. `qemu_trace_opts`

**Network (can disable initially):**
8. `net_init_clients`
9. `net_check_clients`
10. `net_cleanup`
11. `net_client_parse`
12. `netdev_is_modern`
13. `netdev_parse_modern`
14. `qemu_netdev_opts`
15. `qemu_net_opts`
16. `qemu_nic_opts`

**QMP Commands (for runtime control - not boot critical):**
17. `qmp_blockdev_add`
18. `qmp_cont`
19. `qmp_device_add`
20. `qmp_marshal_set_action`
21. `qmp_migrate_incoming`

**QAPI Events (for monitoring - not critical):**
22-27. All `qapi_event_send_*` functions (6 total)

**Block Cleanup:**
28. `blk_exp_close_all`
29. `blk_remove_all_bs`
30. `blockdev_close_all_bdrv_states`
31. `drive_check_orphaned`

**Job System:**
32. `job_cancel_sync_all`
33. `job_next`

**Timers:**
34. `timer_mod_anticipate_ns`
35. `qemu_clock_enable`
36. `icount_configure`

**NUMA:**
37. `numa_complete_configuration`
38. `numa_uses_legacy_mem`
39. `parse_numa_opts`
40. `qemu_numa_opts`

**Memory Backend:**
41. `host_memory_backend_get_memory`
42. `host_memory_backend_is_mapped`
43. `host_memory_backend_set_mapped`
44. `vmstate_register_ram_global`

**Semihosting:**
45-50. All `semihosting_*` and `qemu_semihosting_*` functions (6 total)

---

### PRIORITY 4 - LOW (Can safely stub) - ~54 references

**Trace System (can all be stubbed):**
1-11. All trace functions and DSTATE variables (11 total)

**Debug/Testing:**
12. `gdb_exit`
13. `gdbserver_start`
14. `qtest_driver`
15. `qtest_server_init`

**OS-specific (Switch doesn't support these):**
16. `os_set_chroot`
17. `os_set_daemonize`
18. `os_set_proc_name`
19. `os_set_runas`
20. `os_setup_limits`

**Spice (not used):**
21. `qemu_spice`
22. `using_spice`

**Character devices (advanced):**
23. `qemu_chr_new_from_opts`
24. `qemu_chr_parse_compat`
25. `qemu_chardev_opts`
26. `term_escape_char`

**Monitor (advanced):**
27. `monitor_fdset_add_fd`

**Snapshot/Save State (can defer):**
28. `load_snapshot`
29. `load_snapshot_resume`
30. `dump_vmstate_json_to_file`

**Boot order:**
31. `qemu_boot_set`
32. `restore_boot_order`

**USB:**
33. `usbdevice_create`

**ACPI:**
34. `visit_type_AcpiTableOptions`
35. `qapi_free_AcpiTableOptions`

**Compat Policy:**
36. `visit_type_CompatPolicy`
37. `visit_type_CompatPolicy_members`
38. `qapi_free_CompatPolicy`

**SMBIOS:**
39. `smbios_entry_add`

**Misc:**
40. `override_max_devs`
41. `resume_mux_open`
42. `suspend_mux_open`

---

## Implementation Strategy

### Phase 1: Critical Stubs (Priority 1) - ~2-3 hours
Create stubs for all Priority 1 functions in `switch/qemu-stubs.c`. These are the bare minimum needed to link and potentially boot.

**Key files to check for real implementations:**
- `system/cpus.c` - CPU and main loop functions
- `accel/tcg/` - TCG accelerator
- `ui/console.c` - Display functions
- `block/` - Block device functions
- `hw/xbox/` - xemu-specific functions

### Phase 2: Config & QAPI (Priority 2) - ~2-3 hours
Add QAPI visitor stubs and config option variables. Many can point to static empty structures.

**Strategy:**
- Create dummy QemuOptsList structures for all `qemu_*_opts`
- Stub QAPI visitors to return success without doing anything
- Add minimal monitor infrastructure

### Phase 3: Subsystem Stubs (Priority 3) - ~2 hours
Stub out larger subsystems that aren't needed initially:
- Replay system (all no-ops)
- Network (disabled)
- Audio (silent)
- QMP commands (return errors)

### Phase 4: Final Cleanup (Priority 4) - ~1 hour
Add remaining stubs for trace, debug, and platform-specific functions.

---

## Recommended Next Steps

1. **Start with Priority 1 Critical stubs** - Add these to `switch/qemu-stubs.c`
   - System core functions
   - Accelerator (TCG)
   - Display
   - Block/Drive
   - xemu specific

2. **Find real implementations where possible:**
   - `xbox_eeprom_generate` - likely in `hw/xbox/`
   - `xemu_net_*` - likely in `hw/xbox/` or `ui/`
   - Some QemuOpts might be in existing source files that aren't being compiled

3. **Test incrementally:**
   - Add stubs in batches of 10-20
   - Rebuild to verify link progress
   - Check for new errors introduced by stubs

4. **Document stub behavior:**
   - Mark which stubs are temporary vs permanent
   - Note which ones might need real implementations later

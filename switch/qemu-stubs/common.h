/*
 * QEMU Stubs for Nintendo Switch - Common Header
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef SWITCH_QEMU_STUBS_COMMON_H
#define SWITCH_QEMU_STUBS_COMMON_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>
#include <sys/types.h>
#include <signal.h>
#include <SDL.h>

#include "qemu/osdep.h"
#include "qemu/typedefs.h"
#include "qemu-main.h"
#include "sysemu/arch_init.h"
#include "ui/qemu-spice-module.h"
#include "qapi/error.h"
#include "qapi/qmp/qobject.h"
#include "qapi/qmp/qnull.h"
#include "qapi/visitor.h"
#include "qemu/queue.h"
#include "qemu/option.h"
#include "qapi/util.h"
#include "qapi/qapi-types-machine.h"
#include "hw/block/fdc.h"
#include "hw/qdev-properties.h"

/* Forward type declarations */
typedef struct BlockdevOptions BlockdevOptions;
typedef struct ObjectOptions ObjectOptions;
typedef struct CompatPolicy CompatPolicy;
typedef struct FWCfgState FWCfgState;
typedef struct DriveInfo DriveInfo;
typedef struct DisplayOptions DisplayOptions;
typedef struct DisplayState DisplayState;
typedef struct strList strList;
typedef struct MigrationChannelList MigrationChannelList;
typedef struct MachineState MachineState;
typedef struct DeviceClass DeviceClass;
typedef struct _GMainContext GMainContext;
typedef struct Job Job;
typedef struct BdrvNextIterator BdrvNextIterator;
typedef struct q_obj_SHUTDOWN_arg q_obj_SHUTDOWN_arg;
typedef struct q_obj_RESET_arg q_obj_RESET_arg;
typedef struct AcpiDevAmlIf AcpiDevAmlIf;
typedef struct Aml Aml;
typedef struct ObjectProperty ObjectProperty;
typedef struct CXLState CXLState;
typedef struct HotplugHandler HotplugHandler;
typedef struct MC146818RtcState MC146818RtcState;
typedef struct MemoryDeviceState MemoryDeviceState;
typedef struct NVDIMMState NVDIMMState;
typedef struct PCMachineState PCMachineState;
typedef struct PCDIMMDevice PCDIMMDevice;
typedef struct VirtIOMDPCI VirtIOMDPCI;
typedef struct X86IOMMUState X86IOMMUState;
typedef struct X86MachineState X86MachineState;
typedef struct Audiodev Audiodev;

typedef void (ObjectPropertyAccessor)(Object *obj,
                                      Visitor *v,
                                      const char *name,
                                      void *opaque,
                                      Error **errp);
typedef void (ObjectPropertyRelease)(Object *obj,
                                     const char *name,
                                     void *opaque);

typedef enum BlockInterfaceType {
    IF_DEFAULT = -1,
    IF_NONE = 0,
    IF_IDE,
    IF_SCSI,
    IF_FLOPPY,
    IF_PFLASH,
    IF_MTD,
    IF_SD,
    IF_VIRTIO,
    IF_XEN,
    IF_COUNT
} BlockInterfaceType;

struct DriveInfo {
    BlockInterfaceType type;
    int bus;
    int unit;
    int auto_del;
    bool is_default;
    int media_cd;
    QemuOpts *opts;
    bool locked;
    QTAILQ_ENTRY(DriveInfo) next;
};

typedef struct Chardev Chardev;
typedef struct Monitor Monitor;
typedef struct JSONWriter JSONWriter;
typedef struct VMStateField VMStateField;
typedef struct VMStateInfo {
    const char *name;
    int (*get)(QEMUFile *f, void *pv, size_t size,
               const VMStateField *field);
    int (*put)(QEMUFile *f, void *pv, size_t size,
               const VMStateField *field,
               JSONWriter *vmdesc);
} VMStateInfo;

typedef struct MemoryRegionOps {
    uint64_t _dummy[8];
} MemoryRegionOps;

typedef int ICountMode;

/* Macro for stubbing integer-returning functions */
#define SWITCH_STUB_INT(name) __attribute__((weak)) int name(void) { switch_log("STUB: " #name " called\n"); return 0; }

/* Utility functions */
void switch_log(const char *format, ...);
bool switch_is_main_thread(void);

#endif /* SWITCH_QEMU_STUBS_COMMON_H */

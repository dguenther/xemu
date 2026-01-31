/*
 * QEMU Stubs for Nintendo Switch - Block Device System
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

typedef struct BlockBackend BlockBackend;
typedef struct BlockAcctStats BlockAcctStats;
typedef struct BlockAIOCB BlockAIOCB;
typedef struct QEMUIOVector QEMUIOVector;
typedef struct QEMUBH QEMUBH;
typedef struct BlockDriverState BlockDriverState;
typedef struct BlockDevOps BlockDevOps;

enum BlockAcctType {
    BLOCK_ACCT_READ,
    BLOCK_ACCT_WRITE,
    BLOCK_ACCT_FLUSH,
    BLOCK_ACCT_UNMAP,
    BLOCK_ACCT_ZONE_REPORT,
    BLOCK_ACCT_ZONE_MGMT
};

int qdev_device_help(QemuOpts *opts)
{
    (void)opts;
    return 0;
}

DeviceState *qdev_device_add(QemuOpts *opts, Error **errp)
{
    (void)opts;
    (void)errp;
    return NULL;
}

void __attribute__((weak)) blk_iostatus_enable(BlockBackend *blk)
{
    (void)blk;
}

void * __attribute__((weak)) blk_blockalign(BlockBackend *blk, size_t size)
{
    (void)blk;
    return malloc(size);
}

__attribute__((weak)) BlockErrorAction blk_get_error_action(BlockBackend *blk, bool is_read, int error)
{
    (void)blk; (void)is_read; (void)error;
    return 0; /* BLOCK_ERROR_ACTION_REPORT */
}

__attribute__((weak)) void blk_error_action(BlockBackend *blk, BlockErrorAction action, bool is_read, int error)
{
    (void)blk; (void)action; (void)is_read; (void)error;
}


void __attribute__((weak)) blk_inc_in_flight(BlockBackend *blk)
{
    (void)blk;
}

void * __attribute__((weak)) blk_aio_get(const void *aiocb_info, BlockBackend *blk, void *cb, void *opaque)
{
    (void)aiocb_info; (void)blk; (void)cb; (void)opaque;
    return NULL;
}

BlockAIOCB * __attribute__((weak)) blk_aio_pdiscard(BlockBackend *blk, int64_t offset, int64_t bytes, void *cb, void *opaque)
{
    (void)blk; (void)offset; (void)bytes; (void)cb; (void)opaque;
    return NULL;
}

int __attribute__((weak)) blk_co_lock_medium(BlockBackend *blk, bool locked)
{
    (void)blk; (void)locked;
    return 0;
}

void __attribute__((weak)) blk_co_eject(BlockBackend *blk, bool eject_flag)
{
    (void)blk; (void)eject_flag;
}

void __attribute__((weak)) blk_dec_in_flight(BlockBackend *blk)
{
    (void)blk;
}

int __attribute__((weak)) blk_attach_dev(BlockBackend *blk, void *dev)
{
    (void)blk; (void)dev;
    return 0;
}

BlockBackend * __attribute__((weak)) blk_by_legacy_dinfo(void *dinfo)
{
    (void)dinfo;
    return NULL;
}

bool __attribute__((weak)) blkconf_blocksizes(void *conf, Error **errp)
{
    (void)conf; (void)errp;
    return true;
}

void __attribute__((weak)) blkconf_locked(void *conf, bool writable)
{
    (void)conf; (void)writable;
}

bool __attribute__((weak)) blkconf_geometry(void *conf, int *trans, unsigned cyls_max, unsigned heads_max, unsigned secs_max, Error **errp)
{
    (void)conf; (void)trans; (void)cyls_max; (void)heads_max; (void)secs_max; (void)errp;
    return true;
}

bool __attribute__((weak)) blkconf_apply_backend_options(void *conf, bool readonly, bool resizable, Error **errp)
{
    (void)conf; (void)readonly; (void)resizable; (void)errp;
    return true;
}

void __attribute__((weak)) blk_exp_close_all(void)
{
}

void __attribute__((weak)) job_cancel_sync_all(void)
{
}

Job * __attribute__((weak)) job_next(Job *job)
{
    (void)job;
    return NULL;
}

void __attribute__((weak)) blk_remove_all_bs(void)
{
}

void __attribute__((weak)) blockdev_close_all_bdrv_states(void)
{
}

BlockDriverState * __attribute__((weak)) bdrv_first(BdrvNextIterator *it)
{
    (void)it;
    return NULL;
}

/* Additional block stubs needed by system/vl.c and blockdev.c */
int __attribute__((weak)) bdrv_snapshot_delete(BlockDriverState *bs, const char *snapshot_id, const char *name, Error **errp)
{
    (void)bs; (void)snapshot_id; (void)name; (void)errp;
    return 0;
}

void __attribute__((weak)) job_cancel_sync(Job *job, bool force)
{
    (void)job; (void)force;
}

void __attribute__((weak)) job_start(Job *job)
{
    (void)job;
}

void __attribute__((weak)) timed_average_init(void *ta, void *clock_type, uint64_t period)
{
    (void)ta; (void)clock_type; (void)period;
}

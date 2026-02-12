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

void __attribute__((weak)) blk_iostatus_enable(BlockBackend *blk)
{
    switch_log("STUB: blk_iostatus_enable called\n");
    (void)blk;
}

void * __attribute__((weak)) blk_blockalign(BlockBackend *blk, size_t size)
{
    (void)blk;
    return malloc(size);
}

__attribute__((weak)) BlockErrorAction blk_get_error_action(BlockBackend *blk, bool is_read, int error)
{
    switch_log("STUB: blk_get_error_action called\n");
    (void)blk; (void)is_read; (void)error;
    return 0; /* BLOCK_ERROR_ACTION_REPORT */
}

__attribute__((weak)) void blk_error_action(BlockBackend *blk, BlockErrorAction action, bool is_read, int error)
{
    switch_log("STUB: blk_error_action called\n");
    (void)blk; (void)action; (void)is_read; (void)error;
}


void __attribute__((weak)) blk_inc_in_flight(BlockBackend *blk)
{
    switch_log("STUB: blk_inc_in_flight called\n");
    (void)blk;
}

void * __attribute__((weak)) blk_aio_get(const void *aiocb_info, BlockBackend *blk, void *cb, void *opaque)
{
    switch_log("STUB: blk_aio_get called\n");
    (void)aiocb_info; (void)blk; (void)cb; (void)opaque;
    return NULL;
}

BlockAIOCB * __attribute__((weak)) blk_aio_pdiscard(BlockBackend *blk, int64_t offset, int64_t bytes, void *cb, void *opaque)
{
    switch_log("STUB: blk_aio_pdiscard called\n");
    (void)blk; (void)offset; (void)bytes; (void)cb; (void)opaque;
    return NULL;
}

int __attribute__((weak)) blk_co_lock_medium(BlockBackend *blk, bool locked)
{
    switch_log("STUB: blk_co_lock_medium called\n");
    (void)blk; (void)locked;
    return 0;
}

void __attribute__((weak)) blk_co_eject(BlockBackend *blk, bool eject_flag)
{
    switch_log("STUB: blk_co_eject called\n");
    (void)blk; (void)eject_flag;
}

void __attribute__((weak)) blk_dec_in_flight(BlockBackend *blk)
{
    switch_log("STUB: blk_dec_in_flight called\n");
    (void)blk;
}

int __attribute__((weak)) blk_attach_dev(BlockBackend *blk, void *dev)
{
    switch_log("STUB: blk_attach_dev called\n");
    (void)blk; (void)dev;
    return 0;
}

BlockBackend * __attribute__((weak)) blk_by_legacy_dinfo(void *dinfo)
{
    switch_log("STUB: blk_by_legacy_dinfo called\n");
    (void)dinfo;
    return NULL;
}

bool __attribute__((weak)) blkconf_blocksizes(void *conf, Error **errp)
{
    switch_log("STUB: blkconf_blocksizes called\n");
    (void)conf; (void)errp;
    return true;
}

void __attribute__((weak)) blkconf_locked(void *conf, bool writable)
{
    switch_log("STUB: blkconf_locked called\n");
    (void)conf; (void)writable;
}

bool __attribute__((weak)) blkconf_geometry(void *conf, int *trans, unsigned cyls_max, unsigned heads_max, unsigned secs_max, Error **errp)
{
    switch_log("STUB: blkconf_geometry called\n");
    (void)conf; (void)trans; (void)cyls_max; (void)heads_max; (void)secs_max; (void)errp;
    return true;
}

bool __attribute__((weak)) blkconf_apply_backend_options(void *conf, bool readonly, bool resizable, Error **errp)
{
    switch_log("STUB: blkconf_apply_backend_options called\n");
    (void)conf; (void)readonly; (void)resizable; (void)errp;
    return true;
}

void __attribute__((weak)) blk_exp_close_all(void)
{
    switch_log("STUB: blk_exp_close_all called\n");
}

void __attribute__((weak)) job_cancel_sync_all(void)
{
    switch_log("STUB: job_cancel_sync_all called\n");
}

Job * __attribute__((weak)) job_next(Job *job)
{
    switch_log("STUB: job_next called\n");
    (void)job;
    return NULL;
}

void __attribute__((weak)) blk_remove_all_bs(void)
{
    switch_log("STUB: blk_remove_all_bs called\n");
}

void __attribute__((weak)) blockdev_close_all_bdrv_states(void)
{
    switch_log("STUB: blockdev_close_all_bdrv_states called\n");
}

BlockDriverState * __attribute__((weak)) bdrv_first(BdrvNextIterator *it)
{
    switch_log("STUB: bdrv_first called\n");
    (void)it;
    return NULL;
}

/* Additional block stubs needed by system/vl.c and blockdev.c */
int __attribute__((weak)) bdrv_snapshot_delete(BlockDriverState *bs, const char *snapshot_id, const char *name, Error **errp)
{
    switch_log("STUB: bdrv_snapshot_delete called\n");
    (void)bs; (void)snapshot_id; (void)name; (void)errp;
    return 0;
}

void __attribute__((weak)) job_cancel_sync(Job *job, bool force)
{
    switch_log("STUB: job_cancel_sync called\n");
    (void)job; (void)force;
}

void __attribute__((weak)) job_start(Job *job)
{
    switch_log("STUB: job_start called\n");
    (void)job;
}

void __attribute__((weak)) timed_average_init(void *ta, void *clock_type, uint64_t period)
{
    switch_log("STUB: timed_average_init called\n");
    (void)ta; (void)clock_type; (void)period;
}

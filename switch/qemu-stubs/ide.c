/*
 * QEMU Stubs for Nintendo Switch - IDE/ATAPI/CD-ROM
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

typedef struct QEMUBH QEMUBH;
typedef void QEMUBHFunc(void *opaque);

/* Bottom-half stubs */

QEMUBH *qemu_bh_new_full(QEMUBHFunc *cb, void *opaque, const char *name)
{
    (void)cb; (void)opaque; (void)name;
    return NULL;
}

/* CD-ROM TOC functions */
int cdrom_read_toc(int nb_sectors, uint8_t *buf, int msf, int start_track)
{
    (void)nb_sectors; (void)buf; (void)msf; (void)start_track;
    return 0;
}

int cdrom_read_toc_raw(int nb_sectors, uint8_t *buf, int msf, int session_num)
{
    (void)nb_sectors; (void)buf; (void)msf; (void)session_num;
    return 0;
}

void add_boot_device_lchs(void *dev, const char *suffix, uint32_t lcyls, uint32_t lheads, uint32_t lsecs)
{
    (void)dev; (void)suffix; (void)lcyls; (void)lheads; (void)lsecs;
}

void add_boot_device_path(int32_t bootindex, void *dev, const char *suffix)
{
    (void)bootindex; (void)dev; (void)suffix;
}

void check_boot_index(int32_t bootindex, Error **errp)
{
    (void)bootindex; (void)errp;
}

/*
 * QEMU Stubs for Nintendo Switch - IDE/ATAPI/CD-ROM
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"
#include "block/aio.h"
#include "qemu/main-loop.h"

/* Bottom-half wrapper for IDE restart path */

/* CD-ROM TOC functions */
int cdrom_read_toc(int nb_sectors, uint8_t *buf, int msf, int start_track)
{
    switch_log("STUB: cdrom_read_toc called\n");
    (void)nb_sectors; (void)buf; (void)msf; (void)start_track;
    return 0;
}

int cdrom_read_toc_raw(int nb_sectors, uint8_t *buf, int msf, int session_num)
{
    switch_log("STUB: cdrom_read_toc_raw called\n");
    (void)nb_sectors; (void)buf; (void)msf; (void)session_num;
    return 0;
}

/*
 * QEMU Stubs for Nintendo Switch - Crypto
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * Crypto block stubs
 */
typedef struct QCryptoBlock QCryptoBlock;
typedef struct QCryptoBlockAmendOptions QCryptoBlockAmendOptions;

__attribute__((weak)) int qcrypto_block_amend_options(QCryptoBlock *block,
                                                      QCryptoBlockAmendOptions *options,
                                                      Error **errp)
{
    switch_log("STUB: qcrypto_block_amend_options called\n");
    (void)block;
    (void)options;
    (void)errp;
    return -1;
}

__attribute__((weak)) uint64_t qcrypto_block_get_sector_size(QCryptoBlock *block)
{
    switch_log("STUB: qcrypto_block_get_sector_size called\n");
    (void)block;
    return 512;
}

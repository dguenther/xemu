/*
 * noc_file_dialog stub for Nintendo Switch
 *
 * File dialogs are not supported on Switch. This stub provides
 * a no-op implementation that always returns NULL (cancelled).
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <stddef.h>

/* NOC_FILE_DIALOG flags - must match noc_file_dialog.h */
enum {
    NOC_FILE_DIALOG_OPEN    = 1 << 0,
    NOC_FILE_DIALOG_SAVE    = 1 << 1,
    NOC_FILE_DIALOG_DIR     = 1 << 2,
    NOC_FILE_DIALOG_OVERWRITE_CONFIRMATION = 1 << 3,
};

/*
 * Stub implementation that always returns NULL (user cancelled).
 * On Switch, users will need to use pre-configured paths or
 * an alternative file browser implementation.
 */
const char *noc_file_dialog_open(int flags,
                                  const char *filters,
                                  const char *default_path,
                                  const char *default_name)
{
    (void)flags;
    (void)filters;
    (void)default_path;
    (void)default_name;

    /* File dialogs not supported on Switch - return NULL (cancelled) */
    return NULL;
}

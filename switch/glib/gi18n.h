/*
 * GLib i18n stub for Nintendo Switch
 *
 * Provides minimal stubs for internationalization functions.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef SWITCH_GLIB_GI18N_H
#define SWITCH_GLIB_GI18N_H

/* Internationalization macros - just return the string unchanged */
#define _(String) (String)
#define N_(String) (String)
#define C_(Context, String) (String)
#define Q_(String) (String)

/* gettext() compatibility */
#define gettext(String) (String)
#define dgettext(Domain, String) (String)
#define dcgettext(Domain, String, Type) (String)
#define ngettext(String1, String2, N) ((N) == 1 ? (String1) : (String2))
#define dngettext(Domain, String1, String2, N) ((N) == 1 ? (String1) : (String2))

/* textdomain stubs */
static inline const char *textdomain(const char *domain) {
    (void)domain;
    return "";
}

static inline const char *bindtextdomain(const char *domain, const char *dir) {
    (void)domain;
    (void)dir;
    return "";
}

static inline const char *bind_textdomain_codeset(const char *domain, const char *codeset) {
    (void)domain;
    (void)codeset;
    return "";
}

#endif /* SWITCH_GLIB_GI18N_H */

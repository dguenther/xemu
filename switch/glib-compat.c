/*
 * GLib Compatibility Layer Implementation for Nintendo Switch
 *
 * This file provides implementations of glib-2.0 functions used by QEMU/xemu.
 * It maps glib functions to standard C library equivalents where possible.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "glib-compat.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

/*
 * String functions
 */

gchar* g_strdup(const gchar *str)
{
    if (str == NULL) {
        return NULL;
    }
    gsize len = strlen(str) + 1;
    gchar *new_str = g_malloc(len);
    memcpy(new_str, str, len);
    return new_str;
}

gchar* g_strndup(const gchar *str, gsize n)
{
    if (str == NULL) {
        return NULL;
    }
    gsize len = strlen(str);
    if (n < len) {
        len = n;
    }
    gchar *new_str = g_malloc(len + 1);
    memcpy(new_str, str, len);
    new_str[len] = '\0';
    return new_str;
}

gchar* g_strdup_vprintf(const gchar *format, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);

    int len = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);

    if (len < 0) {
        return NULL;
    }

    gchar *str = g_malloc(len + 1);
    vsnprintf(str, len + 1, format, args);
    return str;
}

gchar* g_strdup_printf(const gchar *format, ...)
{
    va_list args;
    va_start(args, format);
    gchar *str = g_strdup_vprintf(format, args);
    va_end(args);
    return str;
}

gchar* g_strnfill(gsize length, gchar fill_char)
{
    gchar *str = g_malloc(length + 1);
    memset(str, fill_char, length);
    str[length] = '\0';
    return str;
}

gchar* g_strconcat(const gchar *string1, ...)
{
    va_list args;
    gsize total_len = 0;
    const gchar *s;

    /* Calculate total length */
    if (string1) {
        total_len = strlen(string1);
        va_start(args, string1);
        while ((s = va_arg(args, const gchar*)) != NULL) {
            total_len += strlen(s);
        }
        va_end(args);
    }

    gchar *result = g_malloc(total_len + 1);
    gchar *p = result;

    if (string1) {
        gsize len = strlen(string1);
        memcpy(p, string1, len);
        p += len;

        va_start(args, string1);
        while ((s = va_arg(args, const gchar*)) != NULL) {
            len = strlen(s);
            memcpy(p, s, len);
            p += len;
        }
        va_end(args);
    }

    *p = '\0';
    return result;
}

gchar** g_strsplit(const gchar *string, const gchar *delimiter, gint max_tokens)
{
    GPtrArray *array = g_ptr_array_new();
    const gchar *remainder = string;
    gsize delimiter_len = strlen(delimiter);
    gint n_tokens = 0;

    if (max_tokens < 1) {
        max_tokens = G_MAXINT;
    }

    while (remainder && n_tokens < max_tokens - 1) {
        const gchar *next = strstr(remainder, delimiter);
        if (next) {
            g_ptr_array_add(array, g_strndup(remainder, next - remainder));
            remainder = next + delimiter_len;
            n_tokens++;
        } else {
            break;
        }
    }

    if (remainder) {
        g_ptr_array_add(array, g_strdup(remainder));
    }

    g_ptr_array_add(array, NULL);
    return (gchar**)g_ptr_array_free(array, FALSE);
}

void g_strfreev(gchar **str_array)
{
    if (str_array == NULL) {
        return;
    }
    for (gchar **p = str_array; *p != NULL; p++) {
        g_free(*p);
    }
    g_free(str_array);
}

gchar* g_strjoinv(const gchar *separator, gchar **str_array)
{
    if (str_array == NULL || str_array[0] == NULL) {
        return g_strdup("");
    }

    gsize sep_len = separator ? strlen(separator) : 0;
    gsize total_len = 0;
    gint count = 0;

    for (gchar **p = str_array; *p != NULL; p++) {
        total_len += strlen(*p);
        count++;
    }
    total_len += sep_len * (count - 1);

    gchar *result = g_malloc(total_len + 1);
    gchar *dest = result;

    for (gchar **p = str_array; *p != NULL; p++) {
        if (p != str_array && separator) {
            memcpy(dest, separator, sep_len);
            dest += sep_len;
        }
        gsize len = strlen(*p);
        memcpy(dest, *p, len);
        dest += len;
    }
    *dest = '\0';

    return result;
}

gchar* g_strjoin(const gchar *separator, ...)
{
    va_list args;
    GPtrArray *array = g_ptr_array_new();
    const gchar *s;

    va_start(args, separator);
    while ((s = va_arg(args, const gchar*)) != NULL) {
        g_ptr_array_add(array, (gpointer)s);
    }
    va_end(args);

    g_ptr_array_add(array, NULL);
    gchar *result = g_strjoinv(separator, (gchar**)array->pdata);
    g_ptr_array_free(array, TRUE);
    return result;
}

gchar* g_strstrip(gchar *string)
{
    return g_strchomp(g_strchug(string));
}

gchar* g_strchomp(gchar *string)
{
    if (string == NULL) {
        return NULL;
    }
    gsize len = strlen(string);
    while (len > 0 && g_ascii_isspace(string[len - 1])) {
        len--;
    }
    string[len] = '\0';
    return string;
}

gchar* g_strchug(gchar *string)
{
    if (string == NULL) {
        return NULL;
    }
    gchar *p = string;
    while (*p && g_ascii_isspace(*p)) {
        p++;
    }
    if (p != string) {
        memmove(string, p, strlen(p) + 1);
    }
    return string;
}

gchar* g_strreverse(gchar *string)
{
    if (string == NULL) {
        return NULL;
    }
    gsize len = strlen(string);
    for (gsize i = 0; i < len / 2; i++) {
        gchar tmp = string[i];
        string[i] = string[len - 1 - i];
        string[len - 1 - i] = tmp;
    }
    return string;
}

gint g_strcmp0(const gchar *str1, const gchar *str2)
{
    if (str1 == NULL && str2 == NULL) {
        return 0;
    }
    if (str1 == NULL) {
        return -1;
    }
    if (str2 == NULL) {
        return 1;
    }
    return strcmp(str1, str2);
}

gboolean g_str_has_prefix(const gchar *str, const gchar *prefix)
{
    if (str == NULL || prefix == NULL) {
        return FALSE;
    }
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

gboolean g_str_has_suffix(const gchar *str, const gchar *suffix)
{
    if (str == NULL || suffix == NULL) {
        return FALSE;
    }
    gsize str_len = strlen(str);
    gsize suffix_len = strlen(suffix);
    if (suffix_len > str_len) {
        return FALSE;
    }
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

gint g_ascii_strcasecmp(const gchar *s1, const gchar *s2)
{
    while (*s1 && *s2) {
        gint c1 = g_ascii_tolower(*s1);
        gint c2 = g_ascii_tolower(*s2);
        if (c1 != c2) {
            return c1 - c2;
        }
        s1++;
        s2++;
    }
    return g_ascii_tolower(*s1) - g_ascii_tolower(*s2);
}

gint g_ascii_strncasecmp(const gchar *s1, const gchar *s2, gsize n)
{
    while (n > 0 && *s1 && *s2) {
        gint c1 = g_ascii_tolower(*s1);
        gint c2 = g_ascii_tolower(*s2);
        if (c1 != c2) {
            return c1 - c2;
        }
        s1++;
        s2++;
        n--;
    }
    if (n == 0) {
        return 0;
    }
    return g_ascii_tolower(*s1) - g_ascii_tolower(*s2);
}

gchar g_ascii_tolower(gchar c)
{
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

gchar g_ascii_toupper(gchar c)
{
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }
    return c;
}

gchar* g_ascii_strdown(const gchar *str, gssize len)
{
    if (str == NULL) {
        return NULL;
    }
    if (len < 0) {
        len = strlen(str);
    }
    gchar *result = g_malloc(len + 1);
    for (gssize i = 0; i < len; i++) {
        result[i] = g_ascii_tolower(str[i]);
    }
    result[len] = '\0';
    return result;
}

gchar* g_ascii_strup(const gchar *str, gssize len)
{
    if (str == NULL) {
        return NULL;
    }
    if (len < 0) {
        len = strlen(str);
    }
    gchar *result = g_malloc(len + 1);
    for (gssize i = 0; i < len; i++) {
        result[i] = g_ascii_toupper(str[i]);
    }
    result[len] = '\0';
    return result;
}

gboolean g_ascii_isalnum(gchar c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

gboolean g_ascii_isalpha(gchar c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

gboolean g_ascii_isdigit(gchar c)
{
    return c >= '0' && c <= '9';
}

gboolean g_ascii_isspace(gchar c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

gboolean g_ascii_isxdigit(gchar c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

gsize g_strlcpy(gchar *dest, const gchar *src, gsize dest_size)
{
    gsize src_len = strlen(src);
    if (dest_size > 0) {
        gsize copy_len = src_len < dest_size - 1 ? src_len : dest_size - 1;
        memcpy(dest, src, copy_len);
        dest[copy_len] = '\0';
    }
    return src_len;
}

gsize g_strlcat(gchar *dest, const gchar *src, gsize dest_size)
{
    gsize dest_len = strlen(dest);
    gsize src_len = strlen(src);
    if (dest_len >= dest_size) {
        return dest_size + src_len;
    }
    gsize space = dest_size - dest_len - 1;
    gsize copy_len = src_len < space ? src_len : space;
    memcpy(dest + dest_len, src, copy_len);
    dest[dest_len + copy_len] = '\0';
    return dest_len + src_len;
}

gint g_snprintf(gchar *string, gulong n, const gchar *format, ...)
{
    va_list args;
    va_start(args, format);
    gint result = vsnprintf(string, n, format, args);
    va_end(args);
    return result;
}

gint g_vsnprintf(gchar *string, gulong n, const gchar *format, va_list args)
{
    return vsnprintf(string, n, format, args);
}

gint g_sprintf(gchar *string, const gchar *format, ...)
{
    va_list args;
    va_start(args, format);
    gint result = vsprintf(string, format, args);
    va_end(args);
    return result;
}

gint g_vsprintf(gchar *string, const gchar *format, va_list args)
{
    return vsprintf(string, format, args);
}

gint g_printf(const gchar *format, ...)
{
    va_list args;
    va_start(args, format);
    gint result = vprintf(format, args);
    va_end(args);
    return result;
}

gint g_vprintf(const gchar *format, va_list args)
{
    return vprintf(format, args);
}

gint g_fprintf(FILE *file, const gchar *format, ...)
{
    va_list args;
    va_start(args, format);
    gint result = vfprintf(file, format, args);
    va_end(args);
    return result;
}

gint g_vfprintf(FILE *file, const gchar *format, va_list args)
{
    return vfprintf(file, format, args);
}

/*
 * Number conversion
 */

gint64 g_ascii_strtoll(const gchar *nptr, gchar **endptr, guint base)
{
    return strtoll(nptr, endptr, base);
}

guint64 g_ascii_strtoull(const gchar *nptr, gchar **endptr, guint base)
{
    return strtoull(nptr, endptr, base);
}

gdouble g_ascii_strtod(const gchar *nptr, gchar **endptr)
{
    return strtod(nptr, endptr);
}

gchar* g_ascii_dtostr(gchar *buffer, gint buf_len, gdouble d)
{
    g_snprintf(buffer, buf_len, "%.17g", d);
    return buffer;
}

/*
 * Error handling
 */

GError* g_error_new(guint32 domain, gint code, const gchar *format, ...)
{
    va_list args;
    va_start(args, format);
    GError *error = g_error_new_valist(domain, code, format, args);
    va_end(args);
    return error;
}

GError* g_error_new_literal(guint32 domain, gint code, const gchar *message)
{
    GError *error = g_new(GError, 1);
    error->domain = domain;
    error->code = code;
    error->message = g_strdup(message);
    return error;
}

GError* g_error_new_valist(guint32 domain, gint code, const gchar *format, va_list args)
{
    GError *error = g_new(GError, 1);
    error->domain = domain;
    error->code = code;
    error->message = g_strdup_vprintf(format, args);
    return error;
}

void g_error_free(GError *error)
{
    if (error) {
        g_free(error->message);
        g_free(error);
    }
}

GError* g_error_copy(const GError *error)
{
    if (error == NULL) {
        return NULL;
    }
    return g_error_new_literal(error->domain, error->code, error->message);
}

gboolean g_error_matches(const GError *error, guint32 domain, gint code)
{
    return error && error->domain == domain && error->code == code;
}

void g_set_error(GError **err, guint32 domain, gint code, const gchar *format, ...)
{
    if (err == NULL || *err != NULL) {
        return;
    }
    va_list args;
    va_start(args, format);
    *err = g_error_new_valist(domain, code, format, args);
    va_end(args);
}

void g_set_error_literal(GError **err, guint32 domain, gint code, const gchar *message)
{
    if (err == NULL || *err != NULL) {
        return;
    }
    *err = g_error_new_literal(domain, code, message);
}

void g_propagate_error(GError **dest, GError *src)
{
    if (dest == NULL) {
        g_error_free(src);
        return;
    }
    if (*dest != NULL) {
        g_error_free(src);
        return;
    }
    *dest = src;
}

void g_clear_error(GError **err)
{
    if (err && *err) {
        g_error_free(*err);
        *err = NULL;
    }
}

void g_prefix_error(GError **err, const gchar *format, ...)
{
    if (err == NULL || *err == NULL) {
        return;
    }

    va_list args;
    va_start(args, format);
    gchar *prefix = g_strdup_vprintf(format, args);
    va_end(args);

    gchar *old_message = (*err)->message;
    (*err)->message = g_strconcat(prefix, old_message, NULL);
    g_free(prefix);
    g_free(old_message);
}

/*
 * Logging
 */

static GLogLevelFlags g_log_always_fatal = G_LOG_LEVEL_ERROR;

void g_log(const gchar *log_domain, GLogLevelFlags log_level,
           const gchar *format, ...)
{
    va_list args;
    va_start(args, format);
    g_logv(log_domain, log_level, format, args);
    va_end(args);
}

void g_logv(const gchar *log_domain, GLogLevelFlags log_level,
            const gchar *format, va_list args)
{
    const gchar *level_str;
    FILE *out = stderr;

    switch (log_level & G_LOG_LEVEL_MASK) {
    case G_LOG_LEVEL_ERROR:
        level_str = "ERROR";
        break;
    case G_LOG_LEVEL_CRITICAL:
        level_str = "CRITICAL";
        break;
    case G_LOG_LEVEL_WARNING:
        level_str = "WARNING";
        break;
    case G_LOG_LEVEL_MESSAGE:
        level_str = "MESSAGE";
        break;
    case G_LOG_LEVEL_INFO:
        level_str = "INFO";
        break;
    case G_LOG_LEVEL_DEBUG:
        level_str = "DEBUG";
        break;
    default:
        level_str = "LOG";
        break;
    }

    if (log_domain) {
        fprintf(out, "%s-%s: ", log_domain, level_str);
    } else {
        fprintf(out, "** %s: ", level_str);
    }

    vfprintf(out, format, args);
    fprintf(out, "\n");
    fflush(out);

    if (log_level & g_log_always_fatal) {
        abort();
    }
}

guint g_log_set_handler(const gchar *log_domain, GLogLevelFlags log_levels,
                        GLogFunc log_func, gpointer user_data)
{
    (void)log_domain;
    (void)log_levels;
    (void)log_func;
    (void)user_data;
    static guint next_id = 1;
    return next_id++;
}

void g_log_remove_handler(const gchar *log_domain, guint handler_id)
{
    (void)log_domain;
    (void)handler_id;
}

GLogLevelFlags g_log_set_fatal_mask(const gchar *log_domain, GLogLevelFlags fatal_mask)
{
    (void)log_domain;
    GLogLevelFlags old = g_log_always_fatal;
    g_log_always_fatal = fatal_mask;
    return old;
}

GLogLevelFlags g_log_set_always_fatal(GLogLevelFlags fatal_mask)
{
    GLogLevelFlags old = g_log_always_fatal;
    g_log_always_fatal = fatal_mask;
    return old;
}

/*
 * Assertions
 */

void g_assertion_message(const gchar *domain, const gchar *file, int line,
                         const gchar *func, const gchar *message)
{
    fprintf(stderr, "%s:%d:%s: %s: %s\n",
            file, line, func, domain ? domain : "**", message);
    abort();
}

void g_assertion_message_expr(const gchar *domain, const gchar *file, int line,
                              const gchar *func, const gchar *expr)
{
    gchar *msg = g_strdup_printf("assertion failed: (%s)", expr);
    g_assertion_message(domain, file, line, func, msg);
    g_free(msg);
}

void g_assertion_message_cmpnum(const gchar *domain, const gchar *file, int line,
                                const gchar *func, const gchar *expr,
                                long double arg1, const gchar *cmp, long double arg2,
                                char numtype)
{
    gchar *msg;
    if (numtype == 'i') {
        msg = g_strdup_printf("assertion failed (%s): (%lld %s %lld)",
                              expr, (long long)arg1, cmp, (long long)arg2);
    } else {
        msg = g_strdup_printf("assertion failed (%s): (%Lg %s %Lg)",
                              expr, arg1, cmp, arg2);
    }
    g_assertion_message(domain, file, line, func, msg);
    g_free(msg);
}

/*
 * Quark implementation (simple hash-based)
 */

static GHashTable *quark_hash = NULL;
static GPtrArray *quark_strings = NULL;
static guint32 next_quark = 1;

static void ensure_quark_tables(void)
{
    if (quark_hash == NULL) {
        quark_hash = g_hash_table_new(g_str_hash, g_str_equal);
        quark_strings = g_ptr_array_new();
        g_ptr_array_add(quark_strings, NULL); /* quark 0 is invalid */
    }
}

GQuark g_quark_from_string(const gchar *string)
{
    if (string == NULL) {
        return 0;
    }

    ensure_quark_tables();

    gpointer value = g_hash_table_lookup(quark_hash, string);
    if (value != NULL) {
        return (GQuark)(guintptr)value;
    }

    GQuark quark = next_quark++;
    gchar *copy = g_strdup(string);
    g_hash_table_insert(quark_hash, copy, (gpointer)(guintptr)quark);
    g_ptr_array_add(quark_strings, copy);

    return quark;
}

GQuark g_quark_from_static_string(const gchar *string)
{
    if (string == NULL) {
        return 0;
    }

    ensure_quark_tables();

    gpointer value = g_hash_table_lookup(quark_hash, string);
    if (value != NULL) {
        return (GQuark)(guintptr)value;
    }

    GQuark quark = next_quark++;
    g_hash_table_insert(quark_hash, (gchar*)string, (gpointer)(guintptr)quark);
    g_ptr_array_add(quark_strings, (gchar*)string);

    return quark;
}

const gchar* g_quark_to_string(GQuark quark)
{
    if (quark == 0 || quark_strings == NULL || quark >= quark_strings->len) {
        return NULL;
    }
    return g_ptr_array_index(quark_strings, quark);
}

GQuark g_quark_try_string(const gchar *string)
{
    if (string == NULL || quark_hash == NULL) {
        return 0;
    }
    gpointer value = g_hash_table_lookup(quark_hash, string);
    return (GQuark)(guintptr)value;
}

/*
 * GList implementation
 */

static GList* g_list_alloc(void)
{
    return g_new0(GList, 1);
}

GList* g_list_append(GList *list, gpointer data)
{
    GList *new_list = g_list_alloc();
    new_list->data = data;

    if (list == NULL) {
        return new_list;
    }

    GList *last = g_list_last(list);
    last->next = new_list;
    new_list->prev = last;
    return list;
}

GList* g_list_prepend(GList *list, gpointer data)
{
    GList *new_list = g_list_alloc();
    new_list->data = data;
    new_list->next = list;

    if (list) {
        list->prev = new_list;
    }

    return new_list;
}

GList* g_list_insert(GList *list, gpointer data, gint position)
{
    if (position < 0) {
        return g_list_append(list, data);
    }
    if (position == 0) {
        return g_list_prepend(list, data);
    }

    GList *tmp = g_list_nth(list, position);
    if (tmp == NULL) {
        return g_list_append(list, data);
    }

    GList *new_list = g_list_alloc();
    new_list->data = data;
    new_list->prev = tmp->prev;
    new_list->next = tmp;
    tmp->prev->next = new_list;
    tmp->prev = new_list;

    return list;
}

GList* g_list_remove(GList *list, gconstpointer data)
{
    GList *tmp = list;
    while (tmp) {
        if (tmp->data == data) {
            if (tmp->prev) {
                tmp->prev->next = tmp->next;
            }
            if (tmp->next) {
                tmp->next->prev = tmp->prev;
            }
            if (list == tmp) {
                list = tmp->next;
            }
            g_list_free_1(tmp);
            break;
        }
        tmp = tmp->next;
    }
    return list;
}

GList* g_list_remove_link(GList *list, GList *llink)
{
    if (llink == NULL) {
        return list;
    }

    if (llink->prev) {
        llink->prev->next = llink->next;
    }
    if (llink->next) {
        llink->next->prev = llink->prev;
    }
    if (list == llink) {
        list = llink->next;
    }

    llink->prev = NULL;
    llink->next = NULL;

    return list;
}

GList* g_list_delete_link(GList *list, GList *link_)
{
    list = g_list_remove_link(list, link_);
    g_list_free_1(link_);
    return list;
}

GList* g_list_reverse(GList *list)
{
    GList *result = NULL;
    while (list) {
        GList *next = list->next;
        list->next = result;
        list->prev = NULL;
        if (result) {
            result->prev = list;
        }
        result = list;
        list = next;
    }
    return result;
}

GList* g_list_copy(GList *list)
{
    GList *result = NULL;
    while (list) {
        result = g_list_append(result, list->data);
        list = list->next;
    }
    return result;
}

void g_list_free(GList *list)
{
    while (list) {
        GList *next = list->next;
        g_free(list);
        list = next;
    }
}

void g_list_free_full(GList *list, void (*free_func)(gpointer))
{
    while (list) {
        GList *next = list->next;
        if (free_func && list->data) {
            free_func(list->data);
        }
        g_free(list);
        list = next;
    }
}

void g_list_free_1(GList *list)
{
    g_free(list);
}

guint g_list_length(GList *list)
{
    guint count = 0;
    while (list) {
        count++;
        list = list->next;
    }
    return count;
}

GList* g_list_nth(GList *list, guint n)
{
    while (list && n > 0) {
        list = list->next;
        n--;
    }
    return list;
}

gpointer g_list_nth_data(GList *list, guint n)
{
    GList *node = g_list_nth(list, n);
    return node ? node->data : NULL;
}

GList* g_list_find(GList *list, gconstpointer data)
{
    while (list) {
        if (list->data == data) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}

GList* g_list_first(GList *list)
{
    if (list == NULL) {
        return NULL;
    }
    while (list->prev) {
        list = list->prev;
    }
    return list;
}

GList* g_list_last(GList *list)
{
    if (list == NULL) {
        return NULL;
    }
    while (list->next) {
        list = list->next;
    }
    return list;
}

void g_list_foreach(GList *list, void (*func)(gpointer, gpointer), gpointer user_data)
{
    while (list) {
        func(list->data, user_data);
        list = list->next;
    }
}

/*
 * GSList implementation
 */

GSList* g_slist_append(GSList *list, gpointer data)
{
    GSList *new_list = g_new(GSList, 1);
    new_list->data = data;
    new_list->next = NULL;

    if (list == NULL) {
        return new_list;
    }

    GSList *last = g_slist_last(list);
    last->next = new_list;
    return list;
}

GSList* g_slist_prepend(GSList *list, gpointer data)
{
    GSList *new_list = g_new(GSList, 1);
    new_list->data = data;
    new_list->next = list;
    return new_list;
}

GSList* g_slist_remove(GSList *list, gconstpointer data)
{
    GSList *tmp = list;
    GSList *prev = NULL;

    while (tmp) {
        if (tmp->data == data) {
            if (prev) {
                prev->next = tmp->next;
            } else {
                list = tmp->next;
            }
            g_slist_free_1(tmp);
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    return list;
}

void g_slist_free(GSList *list)
{
    while (list) {
        GSList *next = list->next;
        g_free(list);
        list = next;
    }
}

void g_slist_free_full(GSList *list, void (*free_func)(gpointer))
{
    while (list) {
        GSList *next = list->next;
        if (free_func && list->data) {
            free_func(list->data);
        }
        g_free(list);
        list = next;
    }
}

void g_slist_free_1(GSList *list)
{
    g_free(list);
}

guint g_slist_length(GSList *list)
{
    guint count = 0;
    while (list) {
        count++;
        list = list->next;
    }
    return count;
}

GSList* g_slist_last(GSList *list)
{
    if (list == NULL) {
        return NULL;
    }
    while (list->next) {
        list = list->next;
    }
    return list;
}

/*
 * GQueue implementation
 */

GQueue* g_queue_new(void)
{
    return g_new0(GQueue, 1);
}

void g_queue_free(GQueue *queue)
{
    if (queue) {
        g_list_free(queue->head);
        g_free(queue);
    }
}

void g_queue_init(GQueue *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
}

void g_queue_clear(GQueue *queue)
{
    g_list_free(queue->head);
    g_queue_init(queue);
}

gboolean g_queue_is_empty(GQueue *queue)
{
    return queue == NULL || queue->length == 0;
}

guint g_queue_get_length(GQueue *queue)
{
    return queue ? queue->length : 0;
}

void g_queue_push_tail(GQueue *queue, gpointer data)
{
    queue->tail = g_list_append(queue->tail, data);
    if (queue->tail->next) {
        queue->tail = queue->tail->next;
    }
    if (queue->head == NULL) {
        queue->head = queue->tail;
    }
    queue->length++;
}

void g_queue_push_head(GQueue *queue, gpointer data)
{
    queue->head = g_list_prepend(queue->head, data);
    if (queue->tail == NULL) {
        queue->tail = queue->head;
    }
    queue->length++;
}

gpointer g_queue_pop_head(GQueue *queue)
{
    if (queue->head == NULL) {
        return NULL;
    }

    GList *node = queue->head;
    gpointer data = node->data;

    queue->head = node->next;
    if (queue->head) {
        queue->head->prev = NULL;
    } else {
        queue->tail = NULL;
    }

    g_list_free_1(node);
    queue->length--;

    return data;
}

gpointer g_queue_pop_tail(GQueue *queue)
{
    if (queue->tail == NULL) {
        return NULL;
    }

    GList *node = queue->tail;
    gpointer data = node->data;

    queue->tail = node->prev;
    if (queue->tail) {
        queue->tail->next = NULL;
    } else {
        queue->head = NULL;
    }

    g_list_free_1(node);
    queue->length--;

    return data;
}

gpointer g_queue_peek_head(GQueue *queue)
{
    return queue->head ? queue->head->data : NULL;
}

gpointer g_queue_peek_tail(GQueue *queue)
{
    return queue->tail ? queue->tail->data : NULL;
}

/*
 * GHashTable implementation (simple chaining)
 */

#define HASH_TABLE_MIN_SIZE 11

typedef struct _GHashNode GHashNode;
struct _GHashNode {
    gpointer key;
    gpointer value;
    GHashNode *next;
    guint hash;
};

struct _GHashTable {
    gint size;
    gint nnodes;
    GHashNode **nodes;
    GHashFunc hash_func;
    GEqualFunc key_equal_func;
    GDestroyNotify key_destroy_func;
    GDestroyNotify value_destroy_func;
    gint ref_count;
};

GHashTable* g_hash_table_new(GHashFunc hash_func, GEqualFunc key_equal_func)
{
    return g_hash_table_new_full(hash_func, key_equal_func, NULL, NULL);
}

GHashTable* g_hash_table_new_full(GHashFunc hash_func, GEqualFunc key_equal_func,
                                  GDestroyNotify key_destroy_func, GDestroyNotify value_destroy_func)
{
    GHashTable *hash_table = g_new(GHashTable, 1);
    hash_table->size = HASH_TABLE_MIN_SIZE;
    hash_table->nnodes = 0;
    hash_table->nodes = g_new0(GHashNode*, hash_table->size);
    hash_table->hash_func = hash_func ? hash_func : g_direct_hash;
    hash_table->key_equal_func = key_equal_func;
    hash_table->key_destroy_func = key_destroy_func;
    hash_table->value_destroy_func = value_destroy_func;
    hash_table->ref_count = 1;
    return hash_table;
}

void g_hash_table_destroy(GHashTable *hash_table)
{
    if (hash_table == NULL) {
        return;
    }
    g_hash_table_remove_all(hash_table);
    g_free(hash_table->nodes);
    g_free(hash_table);
}

static GHashNode** g_hash_table_lookup_node(GHashTable *hash_table, gconstpointer key, guint *hash_return)
{
    guint hash = hash_table->hash_func(key);
    if (hash_return) {
        *hash_return = hash;
    }

    GHashNode **node = &hash_table->nodes[hash % hash_table->size];

    while (*node) {
        if ((*node)->hash == hash) {
            if (hash_table->key_equal_func) {
                if (hash_table->key_equal_func((*node)->key, key)) {
                    return node;
                }
            } else if ((*node)->key == key) {
                return node;
            }
        }
        node = &(*node)->next;
    }

    return node;
}

gboolean g_hash_table_insert(GHashTable *hash_table, gpointer key, gpointer value)
{
    guint hash;
    GHashNode **node = g_hash_table_lookup_node(hash_table, key, &hash);

    if (*node) {
        /* Key exists, update value */
        if (hash_table->value_destroy_func) {
            hash_table->value_destroy_func((*node)->value);
        }
        (*node)->value = value;
        return FALSE;
    }

    /* New key */
    GHashNode *new_node = g_new(GHashNode, 1);
    new_node->key = key;
    new_node->value = value;
    new_node->hash = hash;
    new_node->next = NULL;
    *node = new_node;
    hash_table->nnodes++;

    return TRUE;
}

gboolean g_hash_table_replace(GHashTable *hash_table, gpointer key, gpointer value)
{
    guint hash;
    GHashNode **node = g_hash_table_lookup_node(hash_table, key, &hash);

    if (*node) {
        if (hash_table->key_destroy_func) {
            hash_table->key_destroy_func((*node)->key);
        }
        if (hash_table->value_destroy_func) {
            hash_table->value_destroy_func((*node)->value);
        }
        (*node)->key = key;
        (*node)->value = value;
        return FALSE;
    }

    GHashNode *new_node = g_new(GHashNode, 1);
    new_node->key = key;
    new_node->value = value;
    new_node->hash = hash;
    new_node->next = NULL;
    *node = new_node;
    hash_table->nnodes++;

    return TRUE;
}

gboolean g_hash_table_remove(GHashTable *hash_table, gconstpointer key)
{
    GHashNode **node = g_hash_table_lookup_node(hash_table, key, NULL);

    if (*node == NULL) {
        return FALSE;
    }

    GHashNode *to_remove = *node;
    *node = to_remove->next;

    if (hash_table->key_destroy_func) {
        hash_table->key_destroy_func(to_remove->key);
    }
    if (hash_table->value_destroy_func) {
        hash_table->value_destroy_func(to_remove->value);
    }
    g_free(to_remove);
    hash_table->nnodes--;

    return TRUE;
}

void g_hash_table_remove_all(GHashTable *hash_table)
{
    for (gint i = 0; i < hash_table->size; i++) {
        GHashNode *node = hash_table->nodes[i];
        while (node) {
            GHashNode *next = node->next;
            if (hash_table->key_destroy_func) {
                hash_table->key_destroy_func(node->key);
            }
            if (hash_table->value_destroy_func) {
                hash_table->value_destroy_func(node->value);
            }
            g_free(node);
            node = next;
        }
        hash_table->nodes[i] = NULL;
    }
    hash_table->nnodes = 0;
}

gpointer g_hash_table_lookup(GHashTable *hash_table, gconstpointer key)
{
    GHashNode **node = g_hash_table_lookup_node(hash_table, key, NULL);
    return *node ? (*node)->value : NULL;
}

gboolean g_hash_table_contains(GHashTable *hash_table, gconstpointer key)
{
    GHashNode **node = g_hash_table_lookup_node(hash_table, key, NULL);
    return *node != NULL;
}

guint g_hash_table_size(GHashTable *hash_table)
{
    return hash_table ? hash_table->nnodes : 0;
}

void g_hash_table_foreach(GHashTable *hash_table, GHFunc func, gpointer user_data)
{
    for (gint i = 0; i < hash_table->size; i++) {
        GHashNode *node = hash_table->nodes[i];
        while (node) {
            func(node->key, node->value, user_data);
            node = node->next;
        }
    }
}

GHashTable* g_hash_table_ref(GHashTable *hash_table)
{
    if (hash_table) {
        hash_table->ref_count++;
    }
    return hash_table;
}

void g_hash_table_unref(GHashTable *hash_table)
{
    if (hash_table && --hash_table->ref_count == 0) {
        g_hash_table_destroy(hash_table);
    }
}

/* Hash functions */
guint g_str_hash(gconstpointer v)
{
    const char *p = v;
    guint h = 0;
    while (*p) {
        h = (h << 5) - h + *p++;
    }
    return h;
}

gboolean g_str_equal(gconstpointer v1, gconstpointer v2)
{
    return strcmp(v1, v2) == 0;
}

guint g_int_hash(gconstpointer v)
{
    return *(const gint*)v;
}

gboolean g_int_equal(gconstpointer v1, gconstpointer v2)
{
    return *(const gint*)v1 == *(const gint*)v2;
}

guint g_int64_hash(gconstpointer v)
{
    const gint64 *val = v;
    return (guint)(*val ^ (*val >> 32));
}

gboolean g_int64_equal(gconstpointer v1, gconstpointer v2)
{
    return *(const gint64*)v1 == *(const gint64*)v2;
}

guint g_direct_hash(gconstpointer v)
{
    return (guint)(guintptr)v;
}

gboolean g_direct_equal(gconstpointer v1, gconstpointer v2)
{
    return v1 == v2;
}

/*
 * GPtrArray implementation
 */

GPtrArray* g_ptr_array_new(void)
{
    return g_ptr_array_sized_new(16);
}

GPtrArray* g_ptr_array_sized_new(guint reserved_size)
{
    GPtrArray *array = g_new(GPtrArray, 1);
    array->pdata = g_new(gpointer, reserved_size);
    array->len = 0;
    return array;
}

gpointer* g_ptr_array_free(GPtrArray *array, gboolean free_seg)
{
    gpointer *data = array->pdata;
    if (free_seg) {
        g_free(data);
        data = NULL;
    }
    g_free(array);
    return data;
}

void g_ptr_array_add(GPtrArray *array, gpointer data)
{
    /* Simple growth - double size when needed */
    array->pdata = g_realloc(array->pdata, sizeof(gpointer) * (array->len + 1));
    array->pdata[array->len++] = data;
}

gpointer g_ptr_array_remove_index(GPtrArray *array, guint index_)
{
    if (index_ >= array->len) {
        return NULL;
    }
    gpointer data = array->pdata[index_];
    for (guint i = index_; i < array->len - 1; i++) {
        array->pdata[i] = array->pdata[i + 1];
    }
    array->len--;
    return data;
}

/*
 * GString implementation
 */

GString* g_string_new(const gchar *init)
{
    GString *string = g_new(GString, 1);
    if (init) {
        string->len = strlen(init);
        string->allocated_len = string->len + 1;
        string->str = g_malloc(string->allocated_len);
        memcpy(string->str, init, string->len + 1);
    } else {
        string->len = 0;
        string->allocated_len = 16;
        string->str = g_malloc(string->allocated_len);
        string->str[0] = '\0';
    }
    return string;
}

GString* g_string_sized_new(gsize dfl_size)
{
    GString *string = g_new(GString, 1);
    string->len = 0;
    string->allocated_len = dfl_size > 0 ? dfl_size : 16;
    string->str = g_malloc(string->allocated_len);
    string->str[0] = '\0';
    return string;
}

gchar* g_string_free(GString *string, gboolean free_segment)
{
    gchar *data = string->str;
    if (free_segment) {
        g_free(data);
        data = NULL;
    }
    g_free(string);
    return data;
}

static void g_string_maybe_expand(GString *string, gsize len)
{
    gsize needed = string->len + len + 1;
    if (needed > string->allocated_len) {
        string->allocated_len = needed * 2;
        string->str = g_realloc(string->str, string->allocated_len);
    }
}

GString* g_string_append(GString *string, const gchar *val)
{
    return g_string_append_len(string, val, -1);
}

GString* g_string_append_len(GString *string, const gchar *val, gssize len)
{
    if (len < 0) {
        len = strlen(val);
    }
    g_string_maybe_expand(string, len);
    memcpy(string->str + string->len, val, len);
    string->len += len;
    string->str[string->len] = '\0';
    return string;
}

GString* g_string_append_c(GString *string, gchar c)
{
    g_string_maybe_expand(string, 1);
    string->str[string->len++] = c;
    string->str[string->len] = '\0';
    return string;
}

void g_string_printf(GString *string, const gchar *format, ...)
{
    va_list args;
    va_start(args, format);
    g_string_vprintf(string, format, args);
    va_end(args);
}

void g_string_vprintf(GString *string, const gchar *format, va_list args)
{
    gchar *tmp = g_strdup_vprintf(format, args);
    g_string_assign(string, tmp);
    g_free(tmp);
}

GString* g_string_assign(GString *string, const gchar *rval)
{
    gsize len = strlen(rval);
    g_string_maybe_expand(string, len - string->len);
    memcpy(string->str, rval, len + 1);
    string->len = len;
    return string;
}

GString* g_string_truncate(GString *string, gsize len)
{
    if (len < string->len) {
        string->len = len;
        string->str[len] = '\0';
    }
    return string;
}

void g_string_append_printf(GString *string, const gchar *format, ...)
{
    va_list args;
    va_start(args, format);
    g_string_append_vprintf(string, format, args);
    va_end(args);
}

void g_string_append_vprintf(GString *string, const gchar *format, va_list args)
{
    gchar *tmp = g_strdup_vprintf(format, args);
    g_string_append(string, tmp);
    g_free(tmp);
}

/*
 * Timer
 */

struct _GTimer {
    struct timespec start;
    struct timespec end;
    gboolean active;
};

GTimer* g_timer_new(void)
{
    GTimer *timer = g_new(GTimer, 1);
    g_timer_start(timer);
    return timer;
}

void g_timer_destroy(GTimer *timer)
{
    g_free(timer);
}

void g_timer_start(GTimer *timer)
{
    clock_gettime(CLOCK_MONOTONIC, &timer->start);
    timer->active = TRUE;
}

void g_timer_stop(GTimer *timer)
{
    clock_gettime(CLOCK_MONOTONIC, &timer->end);
    timer->active = FALSE;
}

void g_timer_reset(GTimer *timer)
{
    g_timer_start(timer);
}

gdouble g_timer_elapsed(GTimer *timer, gulong *microseconds)
{
    struct timespec end;
    if (timer->active) {
        clock_gettime(CLOCK_MONOTONIC, &end);
    } else {
        end = timer->end;
    }

    gdouble elapsed = (end.tv_sec - timer->start.tv_sec) +
                      (end.tv_nsec - timer->start.tv_nsec) / 1000000000.0;

    if (microseconds) {
        *microseconds = (gulong)((elapsed - (gint64)elapsed) * 1000000);
    }

    return elapsed;
}

/*
 * Time utilities
 */

void g_usleep(gulong microseconds)
{
    struct timespec ts;
    ts.tv_sec = microseconds / 1000000;
    ts.tv_nsec = (microseconds % 1000000) * 1000;
    nanosleep(&ts, NULL);
}

gint64 g_get_monotonic_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (gint64)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

gint64 g_get_real_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (gint64)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

/*
 * Random number generator
 */

struct _GRand {
    unsigned int seed;
};

GRand* g_rand_new(void)
{
    GRand *rand_ = g_new(GRand, 1);
    rand_->seed = (unsigned int)time(NULL);
    return rand_;
}

GRand* g_rand_new_with_seed(guint32 seed)
{
    GRand *rand_ = g_new(GRand, 1);
    rand_->seed = seed;
    return rand_;
}

void g_rand_free(GRand *rand_)
{
    g_free(rand_);
}

void g_rand_set_seed(GRand *rand_, guint32 seed)
{
    rand_->seed = seed;
}

guint32 g_rand_int(GRand *rand_)
{
    rand_->seed = rand_->seed * 1103515245 + 12345;
    return (rand_->seed / 65536) % 32768;
}

gint32 g_rand_int_range(GRand *rand_, gint32 begin, gint32 end)
{
    guint32 dist = end - begin;
    return begin + (g_rand_int(rand_) % dist);
}

gdouble g_rand_double(GRand *rand_)
{
    return g_rand_int(rand_) / (gdouble)G_MAXUINT32;
}

static GRand *global_rand = NULL;

guint32 g_random_int(void)
{
    if (!global_rand) {
        global_rand = g_rand_new();
    }
    return g_rand_int(global_rand);
}

gint32 g_random_int_range(gint32 begin, gint32 end)
{
    if (!global_rand) {
        global_rand = g_rand_new();
    }
    return g_rand_int_range(global_rand, begin, end);
}

/*
 * File utilities
 */

gboolean g_file_get_contents(const gchar *filename, gchar **contents, gsize *length, GError **error)
{
    FILE *f = fopen(filename, "rb");
    if (!f) {
        g_set_error(error, 0, errno, "Failed to open file: %s", filename);
        return FALSE;
    }

    fseek(f, 0, SEEK_END);
    gsize len = ftell(f);
    fseek(f, 0, SEEK_SET);

    gchar *data = g_malloc(len + 1);
    if (fread(data, 1, len, f) != len) {
        g_free(data);
        fclose(f);
        g_set_error(error, 0, errno, "Failed to read file: %s", filename);
        return FALSE;
    }

    data[len] = '\0';
    fclose(f);

    *contents = data;
    if (length) {
        *length = len;
    }
    return TRUE;
}

gboolean g_file_set_contents(const gchar *filename, const gchar *contents, gssize length, GError **error)
{
    if (length < 0) {
        length = strlen(contents);
    }

    FILE *f = fopen(filename, "wb");
    if (!f) {
        g_set_error(error, 0, errno, "Failed to open file for writing: %s", filename);
        return FALSE;
    }

    if (fwrite(contents, 1, length, f) != (gsize)length) {
        fclose(f);
        g_set_error(error, 0, errno, "Failed to write file: %s", filename);
        return FALSE;
    }

    fclose(f);
    return TRUE;
}

gboolean g_file_test(const gchar *filename, gint test)
{
    struct stat st;
    if (stat(filename, &st) != 0) {
        return FALSE;
    }

    if (test & G_FILE_TEST_EXISTS) {
        return TRUE;
    }
    if ((test & G_FILE_TEST_IS_REGULAR) && S_ISREG(st.st_mode)) {
        return TRUE;
    }
    if ((test & G_FILE_TEST_IS_DIR) && S_ISDIR(st.st_mode)) {
        return TRUE;
    }

    return FALSE;
}

gchar* g_get_current_dir(void)
{
    gchar *buffer = g_malloc(PATH_MAX);
    if (getcwd(buffer, PATH_MAX) == NULL) {
        g_free(buffer);
        return NULL;
    }
    return buffer;
}

gchar* g_path_get_dirname(const gchar *file_name)
{
    if (file_name == NULL) {
        return g_strdup(".");
    }

    const gchar *last_slash = strrchr(file_name, '/');
    if (last_slash == NULL) {
        return g_strdup(".");
    }
    if (last_slash == file_name) {
        return g_strdup("/");
    }

    return g_strndup(file_name, last_slash - file_name);
}

gchar* g_path_get_basename(const gchar *file_name)
{
    if (file_name == NULL) {
        return g_strdup(".");
    }

    const gchar *last_slash = strrchr(file_name, '/');
    if (last_slash == NULL) {
        return g_strdup(file_name);
    }

    return g_strdup(last_slash + 1);
}

gchar* g_build_filename(const gchar *first_element, ...)
{
    va_list args;
    GString *path = g_string_new(first_element);
    const gchar *element;

    va_start(args, first_element);
    while ((element = va_arg(args, const gchar*)) != NULL) {
        if (path->len > 0 && path->str[path->len - 1] != '/') {
            g_string_append_c(path, '/');
        }
        g_string_append(path, element);
    }
    va_end(args);

    return g_string_free(path, FALSE);
}

/*
 * Environment
 */

const gchar* g_getenv(const gchar *variable)
{
    return getenv(variable);
}

gboolean g_setenv(const gchar *variable, const gchar *value, gboolean overwrite)
{
    return setenv(variable, value, overwrite) == 0;
}

void g_unsetenv(const gchar *variable)
{
    unsetenv(variable);
}

const gchar* g_get_home_dir(void)
{
    static gchar *home_dir = NULL;
    if (home_dir == NULL) {
        const gchar *env = g_getenv("HOME");
        home_dir = g_strdup(env ? env : "sdmc:/");
    }
    return home_dir;
}

const gchar* g_get_tmp_dir(void)
{
    return "sdmc:/switch/xemu/tmp";
}

static gchar *prgname = NULL;

gchar* g_get_prgname(void)
{
    return prgname;
}

void g_set_prgname(const gchar *name)
{
    g_free(prgname);
    prgname = g_strdup(name);
}

/*
 * Atomic operations (using GCC builtins)
 */

gint g_atomic_int_get(const volatile gint *atomic)
{
    return __atomic_load_n(atomic, __ATOMIC_SEQ_CST);
}

void g_atomic_int_set(volatile gint *atomic, gint newval)
{
    __atomic_store_n(atomic, newval, __ATOMIC_SEQ_CST);
}

void g_atomic_int_inc(volatile gint *atomic)
{
    __atomic_add_fetch(atomic, 1, __ATOMIC_SEQ_CST);
}

gboolean g_atomic_int_dec_and_test(volatile gint *atomic)
{
    return __atomic_sub_fetch(atomic, 1, __ATOMIC_SEQ_CST) == 0;
}

gboolean g_atomic_int_compare_and_exchange(volatile gint *atomic, gint oldval, gint newval)
{
    return __atomic_compare_exchange_n(atomic, &oldval, newval, FALSE,
                                       __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

gint g_atomic_int_add(volatile gint *atomic, gint val)
{
    return __atomic_fetch_add(atomic, val, __ATOMIC_SEQ_CST);
}

gpointer g_atomic_pointer_get(const volatile void *atomic)
{
    return __atomic_load_n((void * volatile *)atomic, __ATOMIC_SEQ_CST);
}

void g_atomic_pointer_set(volatile void *atomic, gpointer newval)
{
    __atomic_store_n((void * volatile *)atomic, newval, __ATOMIC_SEQ_CST);
}

gboolean g_atomic_pointer_compare_and_exchange(volatile void *atomic, gpointer oldval, gpointer newval)
{
    return __atomic_compare_exchange_n((void * volatile *)atomic, &oldval, newval, FALSE,
                                       __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

/*
 * GBytes
 */

struct _GBytes {
    gconstpointer data;
    gsize size;
    gint ref_count;
    GDestroyNotify free_func;
};

GBytes* g_bytes_new(gconstpointer data, gsize size)
{
    GBytes *bytes = g_new(GBytes, 1);
    bytes->data = g_memdup2(data, size);
    bytes->size = size;
    bytes->ref_count = 1;
    bytes->free_func = g_free;
    return bytes;
}

GBytes* g_bytes_new_take(gpointer data, gsize size)
{
    GBytes *bytes = g_new(GBytes, 1);
    bytes->data = data;
    bytes->size = size;
    bytes->ref_count = 1;
    bytes->free_func = g_free;
    return bytes;
}

GBytes* g_bytes_ref(GBytes *bytes)
{
    g_atomic_int_inc(&bytes->ref_count);
    return bytes;
}

void g_bytes_unref(GBytes *bytes)
{
    if (g_atomic_int_dec_and_test(&bytes->ref_count)) {
        if (bytes->free_func) {
            bytes->free_func((gpointer)bytes->data);
        }
        g_free(bytes);
    }
}

gconstpointer g_bytes_get_data(GBytes *bytes, gsize *size)
{
    if (size) {
        *size = bytes->size;
    }
    return bytes->data;
}

gsize g_bytes_get_size(GBytes *bytes)
{
    return bytes->size;
}

/*
 * Testing stubs
 */

void g_test_init(int *argc, char ***argv, ...)
{
    (void)argc;
    (void)argv;
}

void g_test_add_func(const gchar *testpath, void (*test_func)(void))
{
    (void)testpath;
    (void)test_func;
}

int g_test_run(void)
{
    return 0;
}

gboolean g_test_slow(void)
{
    return FALSE;
}

/*
 * GLib Unix functions
 */

#include <unistd.h>
#include <fcntl.h>

gboolean g_unix_open_pipe(gint *fds, gint flags, GError **error)
{
    if (pipe(fds) == -1) {
        g_set_error(error, 0, errno, "Failed to create pipe: %s", strerror(errno));
        return FALSE;
    }

    /* Apply FD_CLOEXEC if requested */
    if (flags & FD_CLOEXEC) {
        int fd_flags;
        fd_flags = fcntl(fds[0], F_GETFD);
        if (fd_flags != -1) {
            fcntl(fds[0], F_SETFD, fd_flags | FD_CLOEXEC);
        }
        fd_flags = fcntl(fds[1], F_GETFD);
        if (fd_flags != -1) {
            fcntl(fds[1], F_SETFD, fd_flags | FD_CLOEXEC);
        }
    }

    return TRUE;
}

gboolean g_unix_set_fd_nonblocking(gint fd, gboolean nonblock, GError **error)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        g_set_error(error, 0, errno, "Failed to get fd flags: %s", strerror(errno));
        return FALSE;
    }

    if (nonblock) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }

    if (fcntl(fd, F_SETFL, flags) == -1) {
        g_set_error(error, 0, errno, "Failed to set fd flags: %s", strerror(errno));
        return FALSE;
    }

    return TRUE;
}

guint g_unix_fd_add(gint fd, gint condition, gboolean (*function)(gint, gint, gpointer), gpointer user_data)
{
    /* Stub - not fully implemented for Switch */
    (void)fd;
    (void)condition;
    (void)function;
    (void)user_data;
    return 0;
}

/*
 * GDateTime implementation
 */

#include <time.h>

struct _GDateTime {
    gint ref_count;
    gint year;
    gint month;
    gint day;
    gint hour;
    gint minute;
    gint second;
    gint microsecond;
    gint64 unix_time;
};

struct _GTimeZone {
    gint ref_count;
    gboolean is_utc;
};

GDateTime* g_date_time_new_now_utc(void)
{
    GDateTime *dt = g_new0(GDateTime, 1);
    dt->ref_count = 1;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    dt->unix_time = ts.tv_sec;
    dt->microsecond = ts.tv_nsec / 1000;

    struct tm tm;
    gmtime_r(&ts.tv_sec, &tm);
    dt->year = tm.tm_year + 1900;
    dt->month = tm.tm_mon + 1;
    dt->day = tm.tm_mday;
    dt->hour = tm.tm_hour;
    dt->minute = tm.tm_min;
    dt->second = tm.tm_sec;

    return dt;
}

GDateTime* g_date_time_new_now_local(void)
{
    GDateTime *dt = g_new0(GDateTime, 1);
    dt->ref_count = 1;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    dt->unix_time = ts.tv_sec;
    dt->microsecond = ts.tv_nsec / 1000;

    struct tm tm;
    localtime_r(&ts.tv_sec, &tm);
    dt->year = tm.tm_year + 1900;
    dt->month = tm.tm_mon + 1;
    dt->day = tm.tm_mday;
    dt->hour = tm.tm_hour;
    dt->minute = tm.tm_min;
    dt->second = tm.tm_sec;

    return dt;
}

GDateTime* g_date_time_ref(GDateTime *datetime)
{
    if (datetime) {
        g_atomic_int_inc(&datetime->ref_count);
    }
    return datetime;
}

void g_date_time_unref(GDateTime *datetime)
{
    if (datetime && g_atomic_int_dec_and_test(&datetime->ref_count)) {
        g_free(datetime);
    }
}

gchar* g_date_time_format(GDateTime *datetime, const gchar *format)
{
    struct tm tm = {0};
    tm.tm_year = datetime->year - 1900;
    tm.tm_mon = datetime->month - 1;
    tm.tm_mday = datetime->day;
    tm.tm_hour = datetime->hour;
    tm.tm_min = datetime->minute;
    tm.tm_sec = datetime->second;

    gchar *buf = g_new(gchar, 256);
    strftime(buf, 256, format, &tm);
    return buf;
}

gchar* g_date_time_format_iso8601(GDateTime *datetime)
{
    return g_strdup_printf("%04d-%02d-%02dT%02d:%02d:%02d.%06dZ",
                           datetime->year, datetime->month, datetime->day,
                           datetime->hour, datetime->minute, datetime->second,
                           datetime->microsecond);
}

gint g_date_time_get_year(GDateTime *datetime) { return datetime->year; }
gint g_date_time_get_month(GDateTime *datetime) { return datetime->month; }
gint g_date_time_get_day_of_month(GDateTime *datetime) { return datetime->day; }
gint g_date_time_get_hour(GDateTime *datetime) { return datetime->hour; }
gint g_date_time_get_minute(GDateTime *datetime) { return datetime->minute; }
gint g_date_time_get_second(GDateTime *datetime) { return datetime->second; }
gint g_date_time_get_microsecond(GDateTime *datetime) { return datetime->microsecond; }
gint64 g_date_time_to_unix(GDateTime *datetime) { return datetime->unix_time; }

GTimeZone* g_time_zone_new_utc(void)
{
    GTimeZone *tz = g_new0(GTimeZone, 1);
    tz->ref_count = 1;
    tz->is_utc = TRUE;
    return tz;
}

GTimeZone* g_time_zone_new_local(void)
{
    GTimeZone *tz = g_new0(GTimeZone, 1);
    tz->ref_count = 1;
    tz->is_utc = FALSE;
    return tz;
}

GTimeZone* g_time_zone_ref(GTimeZone *tz)
{
    if (tz) {
        g_atomic_int_inc(&tz->ref_count);
    }
    return tz;
}

void g_time_zone_unref(GTimeZone *tz)
{
    if (tz && g_atomic_int_dec_and_test(&tz->ref_count)) {
        g_free(tz);
    }
}

/*
 * GChecksum - Simple stub implementation
 * For now just returns empty/dummy values since we don't need real checksums on Switch
 */

struct _GChecksum {
    GChecksumType type;
    gchar string_result[65];  /* Large enough for SHA256 hex */
};

gssize g_checksum_type_get_length(GChecksumType checksum_type)
{
    switch (checksum_type) {
    case G_CHECKSUM_MD5: return 16;
    case G_CHECKSUM_SHA1: return 20;
    case G_CHECKSUM_SHA256: return 32;
    case G_CHECKSUM_SHA384: return 48;
    case G_CHECKSUM_SHA512: return 64;
    default: return -1;
    }
}

GChecksum* g_checksum_new(GChecksumType checksum_type)
{
    GChecksum *checksum = g_new0(GChecksum, 1);
    checksum->type = checksum_type;
    memset(checksum->string_result, '0', 32);  /* Dummy MD5-like string */
    checksum->string_result[32] = '\0';
    return checksum;
}

void g_checksum_free(GChecksum *checksum)
{
    g_free(checksum);
}

void g_checksum_reset(GChecksum *checksum)
{
    if (checksum) {
        memset(checksum->string_result, '0', 32);
        checksum->string_result[32] = '\0';
    }
}

void g_checksum_update(GChecksum *checksum, const guchar *data, gssize length)
{
    (void)checksum;
    (void)data;
    (void)length;
    /* Stub - doesn't actually compute anything */
}

const gchar* g_checksum_get_string(GChecksum *checksum)
{
    return checksum ? checksum->string_result : "";
}

void g_checksum_get_digest(GChecksum *checksum, guint8 *buffer, gsize *digest_len)
{
    (void)checksum;
    if (buffer && digest_len) {
        memset(buffer, 0, *digest_len);
    }
}

gchar* g_compute_checksum_for_data(GChecksumType checksum_type, const guchar *data, gsize length)
{
    (void)data;
    (void)length;
    gssize len = g_checksum_type_get_length(checksum_type);
    if (len < 0) return NULL;
    return g_strnfill(len * 2, '0');  /* Return string of zeros */
}

gchar* g_compute_checksum_for_string(GChecksumType checksum_type, const gchar *str, gssize length)
{
    (void)str;
    (void)length;
    return g_compute_checksum_for_data(checksum_type, NULL, 0);
}

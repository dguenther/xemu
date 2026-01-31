/*
 * GLib Compatibility Layer for Nintendo Switch (libnx)
 *
 * This header provides a minimal compatibility layer for glib-2.0
 * functions used by QEMU/xemu. It maps glib functions to standard
 * C library equivalents where possible.
 *
 * This is NOT a full glib implementation - only the subset needed
 * for xemu to compile and run.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef SWITCH_GLIB_COMPAT_H
#define SWITCH_GLIB_COMPAT_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Basic GLib types
 */
typedef char gchar;
typedef short gshort;
typedef long glong;
typedef int gint;
typedef gint gboolean;

typedef unsigned char guchar;
typedef unsigned short gushort;
typedef unsigned long gulong;
typedef unsigned int guint;

typedef float gfloat;
typedef double gdouble;

typedef void* gpointer;
typedef const void* gconstpointer;

typedef size_t gsize;
typedef ssize_t gssize;
typedef intptr_t gintptr;
typedef uintptr_t guintptr;

typedef int8_t gint8;
typedef uint8_t guint8;
typedef int16_t gint16;
typedef uint16_t guint16;
typedef int32_t gint32;
typedef uint32_t guint32;
typedef int64_t gint64;
typedef uint64_t guint64;

typedef guint64 goffset;
typedef guint32 gunichar;
typedef guint16 gunichar2;

/* Pointer-Integer conversion macros */
#define GINT_TO_POINTER(i) ((gpointer)(gintptr)(i))
#define GPOINTER_TO_INT(p) ((gint)(gintptr)(p))
#define GUINT_TO_POINTER(u) ((gpointer)(guintptr)(u))
#define GPOINTER_TO_UINT(p) ((guint)(guintptr)(p))

/*
 * GLib constants
 */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

/* Min/max values */
#define G_MININT8   ((gint8)  -0x80)
#define G_MAXINT8   ((gint8)   0x7f)
#define G_MAXUINT8  ((guint8)  0xff)

#define G_MININT16  ((gint16)  -0x8000)
#define G_MAXINT16  ((gint16)   0x7fff)
#define G_MAXUINT16 ((guint16)  0xffff)

#define G_MININT32  ((gint32)  -0x80000000)
#define G_MAXINT32  ((gint32)   0x7fffffff)
#define G_MAXUINT32 ((guint32)  0xffffffff)

#define G_MININT64  ((gint64)  G_GINT64_CONSTANT(-0x8000000000000000))
#define G_MAXINT64  G_GINT64_CONSTANT(0x7fffffffffffffff)
#define G_MAXUINT64 G_GUINT64_CONSTANT(0xffffffffffffffff)

#define G_MININT    INT_MIN
#define G_MAXINT    INT_MAX
#define G_MAXUINT   UINT_MAX
#define G_MINLONG   LONG_MIN
#define G_MAXLONG   LONG_MAX
#define G_MAXULONG  ULONG_MAX
#define G_MINSSIZE  SSIZE_MIN
#define G_MAXSSIZE  SSIZE_MAX
#define G_MAXSIZE   SIZE_MAX

#define G_USEC_PER_SEC 1000000

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

#define G_GINT64_CONSTANT(val)  (val##LL)
#define G_GUINT64_CONSTANT(val) (val##ULL)

#define G_GINT64_FORMAT  "li"
#define G_GUINT64_FORMAT "lu"
#define G_GSIZE_FORMAT   "zu"
#define G_GSSIZE_FORMAT  "zi"
#define G_GOFFSET_FORMAT G_GINT64_FORMAT

#define G_LITTLE_ENDIAN 1234
#define G_BIG_ENDIAN    4321
#define G_BYTE_ORDER    G_LITTLE_ENDIAN

/* Format macros for printf */
#define G_GINT64_MODIFIER "l"
#define G_GSIZE_MODIFIER  "z"

/*
 * Compiler attributes
 */
#define G_GNUC_PRINTF(format_idx, arg_idx) \
    __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#define G_GNUC_SCANF(format_idx, arg_idx) \
    __attribute__((__format__ (__scanf__, format_idx, arg_idx)))
#define G_GNUC_NORETURN __attribute__((__noreturn__))
#define G_GNUC_CONST __attribute__((__const__))
#define G_GNUC_PURE __attribute__((__pure__))
#define G_GNUC_MALLOC __attribute__((__malloc__))
#define G_GNUC_DEPRECATED __attribute__((__deprecated__))
#define G_GNUC_UNUSED __attribute__((__unused__))
#define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
#define G_GNUC_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define G_GNUC_FALLTHROUGH __attribute__((__fallthrough__))

#define G_NORETURN G_GNUC_NORETURN

#ifndef G_LIKELY
#define G_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#endif
#ifndef G_UNLIKELY
#define G_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#endif

#define G_STATIC_ASSERT(expr) _Static_assert(expr, #expr)
#define G_STATIC_ASSERT_EXPR(expr) ((void)sizeof(char[(expr) ? 1 : -1]))

/*
 * GLib autoptr/autofree macros
 * These provide automatic cleanup for types using GCC's cleanup attribute
 */
#define GLIB_AUTOPTR_FUNC_NAME(TypeName) glib_autoptr_cleanup_##TypeName
#define GLIB_AUTOPTR_CLEAR_FUNC_NAME(TypeName) glib_autoptr_clear_##TypeName
#define GLIB_AUTO_FUNC_NAME(TypeName) glib_auto_cleanup_##TypeName

#define G_DEFINE_AUTOPTR_CLEANUP_FUNC(TypeName, func) \
    G_GNUC_UNUSED static inline void GLIB_AUTOPTR_FUNC_NAME(TypeName)(TypeName **_ptr) { \
        if (*_ptr) { func(*_ptr); *_ptr = NULL; } \
    }

#define G_DEFINE_AUTO_CLEANUP_CLEAR_FUNC(TypeName, func) \
    G_GNUC_UNUSED static inline void GLIB_AUTO_FUNC_NAME(TypeName)(TypeName *_ptr) { \
        func(_ptr); \
    }

#define G_DEFINE_AUTO_CLEANUP_FREE_FUNC(TypeName, func, none) \
    G_GNUC_UNUSED static inline void GLIB_AUTO_FUNC_NAME(TypeName)(TypeName *_ptr) { \
        if (*_ptr != none) func(*_ptr); \
    }

#define g_autoptr(TypeName) \
    __attribute__((cleanup(GLIB_AUTOPTR_FUNC_NAME(TypeName)))) TypeName*

#define g_auto(TypeName) \
    __attribute__((cleanup(GLIB_AUTO_FUNC_NAME(TypeName)))) TypeName

#define g_autofree \
    __attribute__((cleanup(glib_autoptr_cleanup_generic_gfree)))

G_GNUC_UNUSED static inline void glib_autoptr_cleanup_generic_gfree(void *_ptr)
{
    void **ptr = (void **)_ptr;
    if (*ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

/*
 * N_ELEMENTS macro
 */
#define G_N_ELEMENTS(arr) (sizeof(arr) / sizeof((arr)[0]))

/*
 * Stringify macros
 */
#define G_STRINGIFY(macro_or_string) G_STRINGIFY_ARG(macro_or_string)
#define G_STRINGIFY_ARG(contents) #contents

/*
 * Struct member offset
 */
#define G_STRUCT_OFFSET(struct_type, member) \
    ((glong) offsetof(struct_type, member))

/*
 * Memory allocation functions
 */
static inline gpointer g_malloc(gsize n_bytes)
{
    if (G_UNLIKELY(n_bytes == 0)) {
        return NULL;
    }
    gpointer mem = malloc(n_bytes);
    if (G_UNLIKELY(mem == NULL)) {
        fprintf(stderr, "g_malloc: failed to allocate %zu bytes\n", n_bytes);
        abort();
    }
    return mem;
}

static inline gpointer g_malloc0(gsize n_bytes)
{
    if (G_UNLIKELY(n_bytes == 0)) {
        return NULL;
    }
    gpointer mem = calloc(1, n_bytes);
    if (G_UNLIKELY(mem == NULL)) {
        fprintf(stderr, "g_malloc0: failed to allocate %zu bytes\n", n_bytes);
        abort();
    }
    return mem;
}

static inline gpointer g_try_malloc(gsize n_bytes)
{
    if (n_bytes == 0) {
        return NULL;
    }
    return malloc(n_bytes);
}

static inline gpointer g_try_malloc0(gsize n_bytes)
{
    if (n_bytes == 0) {
        return NULL;
    }
    return calloc(1, n_bytes);
}

static inline gpointer g_realloc(gpointer mem, gsize n_bytes)
{
    if (n_bytes == 0) {
        free(mem);
        return NULL;
    }
    gpointer new_mem = realloc(mem, n_bytes);
    if (G_UNLIKELY(new_mem == NULL)) {
        fprintf(stderr, "g_realloc: failed to allocate %zu bytes\n", n_bytes);
        abort();
    }
    return new_mem;
}

static inline gpointer g_try_realloc(gpointer mem, gsize n_bytes)
{
    if (n_bytes == 0) {
        free(mem);
        return NULL;
    }
    return realloc(mem, n_bytes);
}

static inline void g_free(gpointer mem)
{
    free(mem);
}

#ifndef g_clear_pointer
#define g_clear_pointer(pp, destroy) \
    do { \
        __typeof__(*(pp)) *_pp = (pp); \
        __typeof__(*_pp) _p = *_pp; \
        if (_p) { \
            *_pp = NULL; \
            (destroy)(_p); \
        } \
    } while (0)
#endif

#ifndef g_steal_pointer
#define g_steal_pointer(pp) \
    (__extension__ ({ \
        __typeof__(*(pp)) *_pp = (pp); \
        __typeof__(*_pp) _tmp = *_pp; \
        *_pp = NULL; \
        _tmp; \
    }))
#endif

#define g_new(struct_type, n_structs) \
    ((struct_type*) g_malloc(sizeof(struct_type) * (gsize)(n_structs)))

#define g_new0(struct_type, n_structs) \
    ((struct_type*) g_malloc0(sizeof(struct_type) * (gsize)(n_structs)))

#define g_try_new(struct_type, n_structs) \
    ((struct_type*) g_try_malloc(sizeof(struct_type) * (gsize)(n_structs)))

#define g_try_new0(struct_type, n_structs) \
    ((struct_type*) g_try_malloc0(sizeof(struct_type) * (gsize)(n_structs)))

#define g_renew(struct_type, mem, n_structs) \
    ((struct_type*) g_realloc((mem), sizeof(struct_type) * (gsize)(n_structs)))

#define g_try_renew(struct_type, mem, n_structs) \
    ((struct_type*) g_try_realloc((mem), sizeof(struct_type) * (gsize)(n_structs)))

static inline gpointer g_malloc_n(gsize n_blocks, gsize n_block_bytes)
{
    return g_malloc(n_blocks * n_block_bytes);
}

static inline gpointer g_malloc0_n(gsize n_blocks, gsize n_block_bytes)
{
    return g_malloc0(n_blocks * n_block_bytes);
}

static inline gpointer g_realloc_n(gpointer mem, gsize n_blocks, gsize n_block_bytes)
{
    return g_realloc(mem, n_blocks * n_block_bytes);
}

static inline gpointer g_memdup(gconstpointer mem, guint byte_size)
{
    if (mem == NULL || byte_size == 0) {
        return NULL;
    }
    gpointer new_mem = g_malloc(byte_size);
    memcpy(new_mem, mem, byte_size);
    return new_mem;
}

static inline gpointer g_memdup2(gconstpointer mem, gsize byte_size)
{
    if (mem == NULL || byte_size == 0) {
        return NULL;
    }
    gpointer new_mem = g_malloc(byte_size);
    memcpy(new_mem, mem, byte_size);
    return new_mem;
}

/*
 * Memory slices (simplified - just use regular malloc)
 */
static inline gpointer g_slice_alloc(gsize block_size)
{
    return g_malloc(block_size);
}

static inline gpointer g_slice_alloc0(gsize block_size)
{
    return g_malloc0(block_size);
}

static inline void g_slice_free1(gsize block_size, gpointer mem_block)
{
    (void)block_size;
    g_free(mem_block);
}

#define g_slice_new(type)       ((type*) g_slice_alloc(sizeof(type)))
#define g_slice_new0(type)      ((type*) g_slice_alloc0(sizeof(type)))
#define g_slice_free(type, mem) g_slice_free1(sizeof(type), (mem))

/*
 * String functions
 */
gchar* g_strdup(const gchar *str);
gchar* g_strndup(const gchar *str, gsize n);
gchar* g_strdup_printf(const gchar *format, ...) G_GNUC_PRINTF(1, 2);
gchar* g_strdup_vprintf(const gchar *format, va_list args);
gchar* g_strnfill(gsize length, gchar fill_char);
gchar* g_strconcat(const gchar *string1, ...) G_GNUC_NULL_TERMINATED;
gchar** g_strsplit(const gchar *string, const gchar *delimiter, gint max_tokens);
void g_strfreev(gchar **str_array);
guint g_strv_length(gchar **str_array);
gchar** g_strdupv(gchar **str_array);
gboolean g_strv_contains(const gchar * const *strv, const gchar *str);

/* GStrv is a null-terminated array of strings */
typedef gchar** GStrv;

G_DEFINE_AUTO_CLEANUP_FREE_FUNC(GStrv, g_strfreev, NULL)
gchar* g_strjoinv(const gchar *separator, gchar **str_array);
gchar* g_strjoin(const gchar *separator, ...) G_GNUC_NULL_TERMINATED;
gchar* g_strstrip(gchar *string);
gchar* g_strchomp(gchar *string);
gchar* g_strchug(gchar *string);
gchar* g_strreverse(gchar *string);
gchar* g_strup(gchar *string);
gchar* g_strdown(gchar *string);
gint g_strcmp0(const gchar *str1, const gchar *str2);
gboolean g_str_has_prefix(const gchar *str, const gchar *prefix);
gboolean g_str_has_suffix(const gchar *str, const gchar *suffix);
gint g_ascii_strcasecmp(const gchar *s1, const gchar *s2);
gint g_ascii_strncasecmp(const gchar *s1, const gchar *s2, gsize n);
gchar* g_ascii_strdown(const gchar *str, gssize len);
gchar* g_ascii_strup(const gchar *str, gssize len);
gchar g_ascii_tolower(gchar c);
gchar g_ascii_toupper(gchar c);
gboolean g_ascii_isalnum(gchar c);
gboolean g_ascii_isalpha(gchar c);
gboolean g_ascii_isdigit(gchar c);
gboolean g_ascii_isspace(gchar c);
gboolean g_ascii_isxdigit(gchar c);
gchar* g_strstr_len(const gchar *haystack, gssize haystack_len, const gchar *needle);
gchar* g_strrstr(const gchar *haystack, const gchar *needle);
gchar* g_strrstr_len(const gchar *haystack, gssize haystack_len, const gchar *needle);

/* strlcpy/strlcat */
gsize g_strlcpy(gchar *dest, const gchar *src, gsize dest_size);
gsize g_strlcat(gchar *dest, const gchar *src, gsize dest_size);

/* snprintf wrapper */
gint g_snprintf(gchar *string, gulong n, const gchar *format, ...) G_GNUC_PRINTF(3, 4);
gint g_vsnprintf(gchar *string, gulong n, const gchar *format, va_list args);

/* sprintf variants */
gint g_sprintf(gchar *string, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
gint g_vsprintf(gchar *string, const gchar *format, va_list args);

/* Print functions */
gint g_printf(const gchar *format, ...) G_GNUC_PRINTF(1, 2);
gint g_vprintf(const gchar *format, va_list args);
gint g_fprintf(FILE *file, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
gint g_vfprintf(FILE *file, const gchar *format, va_list args);

/*
 * Number conversion
 */
gint64 g_ascii_strtoll(const gchar *nptr, gchar **endptr, guint base);
guint64 g_ascii_strtoull(const gchar *nptr, gchar **endptr, guint base);
gdouble g_ascii_strtod(const gchar *nptr, gchar **endptr);
gchar* g_ascii_dtostr(gchar *buffer, gint buf_len, gdouble d);

/*
 * Pattern matching
 */
gboolean g_pattern_match_simple(const gchar *pattern, const gchar *string);

/*
 * Error handling
 */
typedef struct _GError GError;
struct _GError {
    guint32 domain;
    gint code;
    gchar *message;
};

#define G_DEFINE_QUARK(QN, q_n) \
    guint32 q_n##_quark(void) { static guint32 q = 0; return q; }

GError* g_error_new(guint32 domain, gint code, const gchar *format, ...) G_GNUC_PRINTF(3, 4);
GError* g_error_new_literal(guint32 domain, gint code, const gchar *message);
GError* g_error_new_valist(guint32 domain, gint code, const gchar *format, va_list args);
void g_error_free(GError *error);
GError* g_error_copy(const GError *error);
gboolean g_error_matches(const GError *error, guint32 domain, gint code);
void g_set_error(GError **err, guint32 domain, gint code, const gchar *format, ...) G_GNUC_PRINTF(4, 5);
void g_set_error_literal(GError **err, guint32 domain, gint code, const gchar *message);
void g_propagate_error(GError **dest, GError *src);
void g_clear_error(GError **err);
void g_prefix_error(GError **err, const gchar *format, ...) G_GNUC_PRINTF(2, 3);

/*
 * Logging
 */
typedef enum {
    G_LOG_FLAG_RECURSION = 1 << 0,
    G_LOG_FLAG_FATAL     = 1 << 1,
    G_LOG_LEVEL_ERROR    = 1 << 2,
    G_LOG_LEVEL_CRITICAL = 1 << 3,
    G_LOG_LEVEL_WARNING  = 1 << 4,
    G_LOG_LEVEL_MESSAGE  = 1 << 5,
    G_LOG_LEVEL_INFO     = 1 << 6,
    G_LOG_LEVEL_DEBUG    = 1 << 7,
    G_LOG_LEVEL_MASK     = ~(G_LOG_FLAG_RECURSION | G_LOG_FLAG_FATAL)
} GLogLevelFlags;

typedef void (*GLogFunc)(const gchar *log_domain, GLogLevelFlags log_level,
                         const gchar *message, gpointer user_data);

void g_log(const gchar *log_domain, GLogLevelFlags log_level,
           const gchar *format, ...) G_GNUC_PRINTF(3, 4);
void g_logv(const gchar *log_domain, GLogLevelFlags log_level,
            const gchar *format, va_list args);
guint g_log_set_handler(const gchar *log_domain, GLogLevelFlags log_levels,
                        GLogFunc log_func, gpointer user_data);
void g_log_remove_handler(const gchar *log_domain, guint handler_id);
GLogLevelFlags g_log_set_fatal_mask(const gchar *log_domain, GLogLevelFlags fatal_mask);
GLogLevelFlags g_log_set_always_fatal(GLogLevelFlags fatal_mask);
GLogFunc g_log_set_default_handler(GLogFunc log_func, gpointer user_data);

#define g_error(...)    g_log(NULL, G_LOG_LEVEL_ERROR, __VA_ARGS__)
#define g_critical(...) g_log(NULL, G_LOG_LEVEL_CRITICAL, __VA_ARGS__)
#define g_warning(...)  g_log(NULL, G_LOG_LEVEL_WARNING, __VA_ARGS__)
#define g_message(...)  g_log(NULL, G_LOG_LEVEL_MESSAGE, __VA_ARGS__)
#define g_info(...)     g_log(NULL, G_LOG_LEVEL_INFO, __VA_ARGS__)
#define g_debug(...)    g_log(NULL, G_LOG_LEVEL_DEBUG, __VA_ARGS__)

/*
 * Assertions
 */
void g_assertion_message(const gchar *domain, const gchar *file, int line,
                         const gchar *func, const gchar *message) G_GNUC_NORETURN;
void g_assertion_message_expr(const gchar *domain, const gchar *file, int line,
                              const gchar *func, const gchar *expr) G_GNUC_NORETURN;
void g_assertion_message_cmpnum(const gchar *domain, const gchar *file, int line,
                                const gchar *func, const gchar *expr,
                                long double arg1, const gchar *cmp, long double arg2,
                                char numtype);

#define g_assert(expr) \
    do { if (G_UNLIKELY(!(expr))) \
        g_assertion_message_expr(NULL, __FILE__, __LINE__, __func__, #expr); \
    } while (0)

#define g_assert_not_reached() \
    g_assertion_message(NULL, __FILE__, __LINE__, __func__, "should not be reached")

#define g_assert_cmpint(n1, cmp, n2) \
    do { gint64 __n1 = (n1), __n2 = (n2); \
        if (!(__n1 cmp __n2)) \
            g_assertion_message_cmpnum(NULL, __FILE__, __LINE__, __func__, \
                #n1 " " #cmp " " #n2, __n1, #cmp, __n2, 'i'); \
    } while (0)

#define g_assert_cmpuint(n1, cmp, n2) \
    do { guint64 __n1 = (n1), __n2 = (n2); \
        if (!(__n1 cmp __n2)) \
            g_assertion_message_cmpnum(NULL, __FILE__, __LINE__, __func__, \
                #n1 " " #cmp " " #n2, __n1, #cmp, __n2, 'u'); \
    } while (0)

#define g_assert_null(expr) g_assert((expr) == NULL)
#define g_assert_nonnull(expr) g_assert((expr) != NULL)
#define g_assert_true(expr) g_assert((expr))
#define g_assert_false(expr) g_assert(!(expr))

#define g_return_if_fail(expr) \
    do { if (G_UNLIKELY(!(expr))) { \
        g_warning("assertion '%s' failed", #expr); return; } \
    } while (0)

#define g_return_val_if_fail(expr, val) \
    do { if (G_UNLIKELY(!(expr))) { \
        g_warning("assertion '%s' failed", #expr); return (val); } \
    } while (0)

#define g_warn_if_fail(expr) \
    do { if (G_UNLIKELY(!(expr))) \
        g_warning("assertion '%s' failed", #expr); \
    } while (0)

#define g_warn_if_reached() \
    g_warning("should not be reached")

/*
 * Quark (unique string IDs)
 */
typedef guint32 GQuark;

GQuark g_quark_from_string(const gchar *string);
GQuark g_quark_from_static_string(const gchar *string);
const gchar* g_quark_to_string(GQuark quark);
GQuark g_quark_try_string(const gchar *string);

#define g_intern_string(string) g_quark_to_string(g_quark_from_string(string))
#define g_intern_static_string(string) g_quark_to_string(g_quark_from_static_string(string))

/*
 * Comparison functions (needed before list types)
 */
typedef gint (*GCompareFunc)(gconstpointer a, gconstpointer b);
typedef gint (*GCompareDataFunc)(gconstpointer a, gconstpointer b, gpointer user_data);

/*
 * GList - doubly linked list
 */
typedef struct _GList GList;
struct _GList {
    gpointer data;
    GList *next;
    GList *prev;
};

GList* g_list_append(GList *list, gpointer data);
GList* g_list_prepend(GList *list, gpointer data);
GList* g_list_insert(GList *list, gpointer data, gint position);
GList* g_list_insert_sorted(GList *list, gpointer data, gint (*func)(gconstpointer, gconstpointer));
GList* g_list_insert_before(GList *list, GList *sibling, gpointer data);
GList* g_list_insert_before_link(GList *list, GList *sibling, GList *link_);
GList* g_list_remove(GList *list, gconstpointer data);
GList* g_list_remove_all(GList *list, gconstpointer data);
GList* g_list_remove_link(GList *list, GList *llink);
GList* g_list_delete_link(GList *list, GList *link_);
GList* g_list_concat(GList *list1, GList *list2);
GList* g_list_reverse(GList *list);
GList* g_list_copy(GList *list);
GList* g_list_copy_deep(GList *list, gpointer (*func)(gconstpointer, gpointer), gpointer user_data);
void g_list_free(GList *list);

G_DEFINE_AUTOPTR_CLEANUP_FUNC(GList, g_list_free)
void g_list_free_full(GList *list, void (*free_func)(gpointer));
void g_list_free_1(GList *list);
guint g_list_length(GList *list);
GList* g_list_nth(GList *list, guint n);
gpointer g_list_nth_data(GList *list, guint n);
GList* g_list_find(GList *list, gconstpointer data);
GList* g_list_find_custom(GList *list, gconstpointer data, gint (*func)(gconstpointer, gconstpointer));
gint g_list_position(GList *list, GList *llink);
gint g_list_index(GList *list, gconstpointer data);
GList* g_list_first(GList *list);
GList* g_list_last(GList *list);
void g_list_foreach(GList *list, void (*func)(gpointer, gpointer), gpointer user_data);
GList* g_list_sort(GList *list, gint (*compare_func)(gconstpointer, gconstpointer));

#define g_list_free1 g_list_free_1
#define g_list_next(list) ((list) ? (list)->next : NULL)
#define g_list_previous(list) ((list) ? (list)->prev : NULL)

/*
 * GSList - singly linked list
 */
typedef struct _GSList GSList;
struct _GSList {
    gpointer data;
    GSList *next;
};

GSList* g_slist_append(GSList *list, gpointer data);
GSList* g_slist_prepend(GSList *list, gpointer data);
GSList* g_slist_remove(GSList *list, gconstpointer data);
GSList* g_slist_remove_all(GSList *list, gconstpointer data);
GSList* g_slist_delete_link(GSList *list, GSList *link_);
GSList* g_slist_reverse(GSList *list);
GSList* g_slist_copy(GSList *list);
void g_slist_free(GSList *list);
void g_slist_free_full(GSList *list, void (*free_func)(gpointer));
void g_slist_free_1(GSList *list);

G_DEFINE_AUTOPTR_CLEANUP_FUNC(GSList, g_slist_free)
guint g_slist_length(GSList *list);
GSList* g_slist_nth(GSList *list, guint n);
gpointer g_slist_nth_data(GSList *list, guint n);
GSList* g_slist_find(GSList *list, gconstpointer data);
GSList* g_slist_find_custom(GSList *list, gconstpointer data, gint (*func)(gconstpointer, gconstpointer));
gint g_slist_position(GSList *list, GSList *llink);
gint g_slist_index(GSList *list, gconstpointer data);
GSList* g_slist_last(GSList *list);
GSList* g_slist_insert_sorted(GSList *list, gpointer data, GCompareFunc func);
void g_slist_foreach(GSList *list, void (*func)(gpointer, gpointer), gpointer user_data);
GSList* g_slist_sort(GSList *list, gint (*compare_func)(gconstpointer, gconstpointer));

#define g_slist_free1 g_slist_free_1
#define g_slist_next(list) ((list) ? (list)->next : NULL)

/*
 * GQueue - double-ended queue
 */
typedef struct _GQueue GQueue;
struct _GQueue {
    GList *head;
    GList *tail;
    guint length;
};

#define G_QUEUE_INIT { NULL, NULL, 0 }

GQueue* g_queue_new(void);
void g_queue_free(GQueue *queue);
void g_queue_free_full(GQueue *queue, void (*free_func)(gpointer));
void g_queue_init(GQueue *queue);
void g_queue_clear(GQueue *queue);
gboolean g_queue_is_empty(GQueue *queue);
guint g_queue_get_length(GQueue *queue);
void g_queue_push_head(GQueue *queue, gpointer data);
void g_queue_push_tail(GQueue *queue, gpointer data);
gpointer g_queue_pop_head(GQueue *queue);
gpointer g_queue_pop_tail(GQueue *queue);
gpointer g_queue_peek_head(GQueue *queue);
gpointer g_queue_peek_tail(GQueue *queue);
void g_queue_foreach(GQueue *queue, void (*func)(gpointer, gpointer), gpointer user_data);
GList* g_queue_find(GQueue *queue, gconstpointer data);
gboolean g_queue_remove(GQueue *queue, gconstpointer data);

/*
 * GHashTable
 */
typedef struct _GHashTable GHashTable;
typedef gboolean (*GHRFunc)(gpointer key, gpointer value, gpointer user_data);
typedef void (*GHFunc)(gpointer key, gpointer value, gpointer user_data);
typedef guint (*GHashFunc)(gconstpointer key);
typedef gboolean (*GEqualFunc)(gconstpointer a, gconstpointer b);
typedef void (*GDestroyNotify)(gpointer data);

/*
 * GSequence - minimal implementation for ordered unique lists
 */
typedef struct _GSequence GSequence;
typedef struct _GSequenceIter GSequenceIter;

GSequence* g_sequence_new(GDestroyNotify data_destroy);
GSequenceIter* g_sequence_lookup(GSequence *seq, gpointer data,
                                 GCompareDataFunc cmp, gpointer user_data);
GSequenceIter* g_sequence_insert_sorted(GSequence *seq, gpointer data,
                                        GCompareDataFunc cmp, gpointer user_data);
void g_sequence_remove(GSequenceIter *iter);
typedef void (*GFunc)(gpointer data, gpointer user_data);
/* GCompareFunc and GCompareDataFunc defined earlier in file */

GHashTable* g_hash_table_new(GHashFunc hash_func, GEqualFunc key_equal_func);
GHashTable* g_hash_table_new_full(GHashFunc hash_func, GEqualFunc key_equal_func,
                                  GDestroyNotify key_destroy_func, GDestroyNotify value_destroy_func);
void g_hash_table_destroy(GHashTable *hash_table);
gboolean g_hash_table_insert(GHashTable *hash_table, gpointer key, gpointer value);
gboolean g_hash_table_replace(GHashTable *hash_table, gpointer key, gpointer value);
gboolean g_hash_table_remove(GHashTable *hash_table, gconstpointer key);
void g_hash_table_remove_all(GHashTable *hash_table);
gboolean g_hash_table_steal(GHashTable *hash_table, gconstpointer key);
gpointer g_hash_table_lookup(GHashTable *hash_table, gconstpointer key);
gboolean g_hash_table_contains(GHashTable *hash_table, gconstpointer key);
gboolean g_hash_table_lookup_extended(GHashTable *hash_table, gconstpointer lookup_key,
                                      gpointer *orig_key, gpointer *value);
void g_hash_table_foreach(GHashTable *hash_table, GHFunc func, gpointer user_data);
guint g_hash_table_foreach_remove(GHashTable *hash_table, GHRFunc func, gpointer user_data);
guint g_hash_table_size(GHashTable *hash_table);
GList* g_hash_table_get_keys(GHashTable *hash_table);
GList* g_hash_table_get_values(GHashTable *hash_table);
gpointer* g_hash_table_get_keys_as_array(GHashTable *hash_table, guint *length);
GHashTable* g_hash_table_ref(GHashTable *hash_table);
void g_hash_table_unref(GHashTable *hash_table);
gboolean g_hash_table_add(GHashTable *hash_table, gpointer key);

G_DEFINE_AUTOPTR_CLEANUP_FUNC(GHashTable, g_hash_table_unref)

/* Hash functions */
guint g_str_hash(gconstpointer v);
gboolean g_str_equal(gconstpointer v1, gconstpointer v2);
guint g_int_hash(gconstpointer v);
gboolean g_int_equal(gconstpointer v1, gconstpointer v2);
guint g_int64_hash(gconstpointer v);
gboolean g_int64_equal(gconstpointer v1, gconstpointer v2);
guint g_direct_hash(gconstpointer v);
gboolean g_direct_equal(gconstpointer v1, gconstpointer v2);

/* GHashTableIter - iterator for hash tables */
typedef struct {
    gpointer dummy1;
    gpointer dummy2;
    gpointer dummy3;
    int dummy4;
    gboolean dummy5;
    gpointer dummy6;
} GHashTableIter;

void g_hash_table_iter_init(GHashTableIter *iter, GHashTable *hash_table);
gboolean g_hash_table_iter_next(GHashTableIter *iter, gpointer *key, gpointer *value);
void g_hash_table_iter_remove(GHashTableIter *iter);
void g_hash_table_iter_steal(GHashTableIter *iter);
GHashTable* g_hash_table_iter_get_hash_table(GHashTableIter *iter);

/*
 * GPtrArray - array of pointers
 */
typedef struct _GPtrArray GPtrArray;
struct _GPtrArray {
    gpointer *pdata;
    guint len;
};

GPtrArray* g_ptr_array_new(void);
GPtrArray* g_ptr_array_new_with_free_func(GDestroyNotify element_free_func);
GPtrArray* g_ptr_array_new_full(guint reserved_size, GDestroyNotify element_free_func);
GPtrArray* g_ptr_array_sized_new(guint reserved_size);
gpointer* g_ptr_array_free(GPtrArray *array, gboolean free_seg);
void g_ptr_array_unref(GPtrArray *array);
GPtrArray* g_ptr_array_ref(GPtrArray *array);
void g_ptr_array_set_free_func(GPtrArray *array, GDestroyNotify element_free_func);
void g_ptr_array_set_size(GPtrArray *array, gint length);
void g_ptr_array_add(GPtrArray *array, gpointer data);
void g_ptr_array_insert(GPtrArray *array, gint index_, gpointer data);
gboolean g_ptr_array_remove(GPtrArray *array, gpointer data);
gpointer g_ptr_array_remove_index(GPtrArray *array, guint index_);
gpointer g_ptr_array_remove_index_fast(GPtrArray *array, guint index_);
gboolean g_ptr_array_remove_fast(GPtrArray *array, gpointer data);
void g_ptr_array_foreach(GPtrArray *array, void (*func)(gpointer, gpointer), gpointer user_data);
gboolean g_ptr_array_find(GPtrArray *array, gconstpointer needle, guint *index_);
gboolean g_ptr_array_find_with_equal_func(GPtrArray *haystack, gconstpointer needle, GEqualFunc equal_func, gpointer user_data);
void g_ptr_array_sort(GPtrArray *array, gint (*compare_func)(gconstpointer, gconstpointer));

#define g_ptr_array_index(array, index_) ((array)->pdata[index_])

/*
 * GArray - array of arbitrary elements
 */
typedef struct _GArray GArray;
struct _GArray {
    gchar *data;
    guint len;
};

GArray* g_array_new(gboolean zero_terminated, gboolean clear_, guint element_size);
GArray* g_array_sized_new(gboolean zero_terminated, gboolean clear_, guint element_size, guint reserved_size);
gchar* g_array_free(GArray *array, gboolean free_segment);
GArray* g_array_ref(GArray *array);
void g_array_unref(GArray *array);
guint g_array_get_element_size(GArray *array);
GArray* g_array_append_vals(GArray *array, gconstpointer data, guint len);
GArray* g_array_prepend_vals(GArray *array, gconstpointer data, guint len);
GArray* g_array_insert_vals(GArray *array, guint index_, gconstpointer data, guint len);
GArray* g_array_set_size(GArray *array, guint length);
GArray* g_array_remove_index(GArray *array, guint index_);
GArray* g_array_remove_index_fast(GArray *array, guint index_);
GArray* g_array_remove_range(GArray *array, guint index_, guint length);
void g_array_sort(GArray *array, gint (*compare_func)(gconstpointer, gconstpointer));

#define g_array_append_val(a,v)   g_array_append_vals(a, &(v), 1)
#define g_array_prepend_val(a,v)  g_array_prepend_vals(a, &(v), 1)
#define g_array_insert_val(a,i,v) g_array_insert_vals(a, i, &(v), 1)
#define g_array_index(a,t,i)      (((t*)(void*)(a)->data)[(i)])

/*
 * GByteArray
 */
typedef struct _GByteArray GByteArray;
struct _GByteArray {
    guint8 *data;
    guint len;
};

GByteArray* g_byte_array_new(void);
GByteArray* g_byte_array_sized_new(guint reserved_size);
guint8* g_byte_array_free(GByteArray *array, gboolean free_segment);
GByteArray* g_byte_array_ref(GByteArray *array);
void g_byte_array_unref(GByteArray *array);
GByteArray* g_byte_array_append(GByteArray *array, const guint8 *data, guint len);
GByteArray* g_byte_array_prepend(GByteArray *array, const guint8 *data, guint len);
GByteArray* g_byte_array_set_size(GByteArray *array, guint length);
GByteArray* g_byte_array_remove_index(GByteArray *array, guint index_);
GByteArray* g_byte_array_remove_index_fast(GByteArray *array, guint index_);
GByteArray* g_byte_array_remove_range(GByteArray *array, guint index_, guint length);
void g_byte_array_sort(GByteArray *array, gint (*compare_func)(gconstpointer, gconstpointer));

/*
 * GString - growable strings
 */
typedef struct _GString GString;
struct _GString {
    gchar *str;
    gsize len;
    gsize allocated_len;
};

GString* g_string_new(const gchar *init);
GString* g_string_new_len(const gchar *init, gssize len);
GString* g_string_sized_new(gsize dfl_size);
gchar* g_string_free(GString *string, gboolean free_segment);
GString* g_string_assign(GString *string, const gchar *rval);
GString* g_string_truncate(GString *string, gsize len);
GString* g_string_set_size(GString *string, gsize len);
GString* g_string_insert_len(GString *string, gssize pos, const gchar *val, gssize len);
GString* g_string_append(GString *string, const gchar *val);
GString* g_string_append_len(GString *string, const gchar *val, gssize len);
GString* g_string_append_c(GString *string, gchar c);
GString* g_string_append_unichar(GString *string, guint32 wc);
GString* g_string_prepend(GString *string, const gchar *val);
GString* g_string_prepend_c(GString *string, gchar c);
GString* g_string_prepend_len(GString *string, const gchar *val, gssize len);
GString* g_string_insert(GString *string, gssize pos, const gchar *val);
GString* g_string_insert_c(GString *string, gssize pos, gchar c);
GString* g_string_erase(GString *string, gssize pos, gssize len);
GString* g_string_ascii_down(GString *string);
GString* g_string_ascii_up(GString *string);
void g_string_printf(GString *string, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
void g_string_vprintf(GString *string, const gchar *format, va_list args);
void g_string_append_printf(GString *string, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
void g_string_append_vprintf(GString *string, const gchar *format, va_list args);
gboolean g_string_equal(const GString *v, const GString *v2);
guint g_string_hash(const GString *str);

G_GNUC_UNUSED static inline void glib_autoptr_cleanup_GString(GString **_ptr)
{
    if (_ptr && *_ptr) {
        g_string_free(*_ptr, TRUE);
    }
}

/*
 * GRand - random number generator
 */
typedef struct _GRand GRand;

GRand* g_rand_new(void);
GRand* g_rand_new_with_seed(guint32 seed);
GRand* g_rand_new_with_seed_array(const guint32 *seed, guint seed_length);
void g_rand_free(GRand *rand_);
void g_rand_set_seed(GRand *rand_, guint32 seed);
guint32 g_rand_int(GRand *rand_);
gint32 g_rand_int_range(GRand *rand_, gint32 begin, gint32 end);
gdouble g_rand_double(GRand *rand_);
gdouble g_rand_double_range(GRand *rand_, gdouble begin, gdouble end);

/* Global random */
guint32 g_random_int(void);
gint32 g_random_int_range(gint32 begin, gint32 end);
gdouble g_random_double(void);
gdouble g_random_double_range(gdouble begin, gdouble end);
void g_random_set_seed(guint32 seed);

/*
 * Timer
 */
typedef struct _GTimer GTimer;

GTimer* g_timer_new(void);
void g_timer_destroy(GTimer *timer);
void g_timer_start(GTimer *timer);
void g_timer_stop(GTimer *timer);
void g_timer_reset(GTimer *timer);
void g_timer_continue(GTimer *timer);
gdouble g_timer_elapsed(GTimer *timer, gulong *microseconds);
gboolean g_timer_is_active(GTimer *timer);

/*
 * GSource, GMainLoop, GMainContext (stubs for SDL-based event loop)
 */
typedef struct _GSource GSource;
typedef struct _GMainLoop GMainLoop;
typedef struct _GMainContext GMainContext;

typedef enum {
    G_IO_IN   = 1 << 0,
    G_IO_OUT  = 1 << 1,
    G_IO_PRI  = 1 << 2,
    G_IO_ERR  = 1 << 3,
    G_IO_HUP  = 1 << 4,
    G_IO_NVAL = 1 << 5,
} GIOCondition;

GMainLoop* g_main_loop_new(GMainContext *context, gboolean is_running);
void g_main_loop_run(GMainLoop *loop);
void g_main_loop_quit(GMainLoop *loop);
gboolean g_main_loop_is_running(GMainLoop *loop);
void g_main_loop_unref(GMainLoop *loop);
GMainLoop* g_main_loop_ref(GMainLoop *loop);
GMainContext* g_main_loop_get_context(GMainLoop *loop);

GMainContext* g_main_context_new(void);
GMainContext* g_main_context_ref(GMainContext *context);
void g_main_context_unref(GMainContext *context);
GMainContext* g_main_context_default(void);
gboolean g_main_context_iteration(GMainContext *context, gboolean may_block);
gboolean g_main_context_pending(GMainContext *context);
void g_main_context_wakeup(GMainContext *context);
gboolean g_main_context_acquire(GMainContext *context);
void g_main_context_release(GMainContext *context);

void g_source_set_can_recurse(GSource *source, gboolean can_recurse);
gboolean g_source_is_destroyed(GSource *source);

guint g_timeout_add(guint interval, gboolean (*function)(gpointer), gpointer data);
guint g_timeout_add_seconds(guint interval, gboolean (*function)(gpointer), gpointer data);
guint g_idle_add(gboolean (*function)(gpointer), gpointer data);
gboolean g_source_remove(guint tag);

/*
 * File utilities
 */
gboolean g_file_get_contents(const gchar *filename, gchar **contents, gsize *length, GError **error);
gboolean g_file_set_contents(const gchar *filename, const gchar *contents, gssize length, GError **error);
gboolean g_file_test(const gchar *filename, gint test);
gint g_mkstemp(gchar *tmpl);
gchar* g_get_current_dir(void);
gchar* g_path_get_dirname(const gchar *file_name);
gchar* g_path_get_basename(const gchar *file_name);
gchar* g_build_filename(const gchar *first_element, ...) G_GNUC_NULL_TERMINATED;
gchar* g_build_path(const gchar *separator, const gchar *first_element, ...) G_GNUC_NULL_TERMINATED;
gboolean g_path_is_absolute(const gchar *file_name);

/*
 * GMappedFile (simple file-backed buffer)
 */
typedef struct _GMappedFile GMappedFile;

GMappedFile *g_mapped_file_new(const gchar *filename, gboolean writable, GError **error);
GMappedFile *g_mapped_file_new_from_fd(int fd, gboolean writable, GError **error);
GMappedFile *g_mapped_file_ref(GMappedFile *file);
void g_mapped_file_unref(GMappedFile *file);
gsize g_mapped_file_get_length(GMappedFile *file);
gchar *g_mapped_file_get_contents(GMappedFile *file);

#define G_FILE_TEST_EXISTS       (1 << 0)
#define G_FILE_TEST_IS_REGULAR   (1 << 1)
#define G_FILE_TEST_IS_DIR       (1 << 2)
#define G_FILE_TEST_IS_EXECUTABLE (1 << 3)
#define G_FILE_TEST_IS_SYMLINK   (1 << 4)

#define G_DIR_SEPARATOR '/'
#define G_DIR_SEPARATOR_S "/"
#define G_IS_DIR_SEPARATOR(c) ((c) == G_DIR_SEPARATOR)
#define G_SEARCHPATH_SEPARATOR ':'
#define G_SEARCHPATH_SEPARATOR_S ":"

/*
 * Environment
 */
const gchar* g_getenv(const gchar *variable);
gboolean g_setenv(const gchar *variable, const gchar *value, gboolean overwrite);
void g_unsetenv(const gchar *variable);
const gchar* g_get_home_dir(void);
const gchar* g_get_user_name(void);
const gchar* g_get_real_name(void);
const gchar* g_get_host_name(void);
const gchar* g_get_tmp_dir(void);
gchar* g_get_prgname(void);
void g_set_prgname(const gchar *prgname);
gchar** g_get_environ(void);
const gchar* g_environ_getenv(gchar **envp, const gchar *variable);
gchar** g_environ_setenv(gchar **envp, const gchar *variable, const gchar *value, gboolean overwrite);
gchar** g_environ_unsetenv(gchar **envp, const gchar *variable);

/*
 * Testing framework stubs
 */
void g_test_init(int *argc, char ***argv, ...);
void g_test_add_func(const gchar *testpath, void (*test_func)(void));
int g_test_run(void);
void g_test_fail(void);
void g_test_skip(const gchar *msg);
void g_test_message(const gchar *format, ...) G_GNUC_PRINTF(1, 2);
gboolean g_test_slow(void);

/*
 * Base64 encoding
 */
gchar* g_base64_encode(const guchar *data, gsize len);
guchar* g_base64_decode(const gchar *text, gsize *out_len);

/*
 * Checksum
 */
typedef enum {
    G_CHECKSUM_MD5,
    G_CHECKSUM_SHA1,
    G_CHECKSUM_SHA256,
    G_CHECKSUM_SHA384,
    G_CHECKSUM_SHA512
} GChecksumType;

typedef struct _GChecksum GChecksum;

GChecksum* g_checksum_new(GChecksumType checksum_type);
void g_checksum_free(GChecksum *checksum);
void g_checksum_reset(GChecksum *checksum);
void g_checksum_update(GChecksum *checksum, const guchar *data, gssize length);
const gchar* g_checksum_get_string(GChecksum *checksum);
void g_checksum_get_digest(GChecksum *checksum, guint8 *buffer, gsize *digest_len);
gssize g_checksum_type_get_length(GChecksumType checksum_type);
gchar* g_compute_checksum_for_data(GChecksumType checksum_type, const guchar *data, gsize length);
gchar* g_compute_checksum_for_string(GChecksumType checksum_type, const gchar *str, gssize length);

/*
 * Misc utilities
 */
void g_usleep(gulong microseconds);
gint64 g_get_monotonic_time(void);
gint64 g_get_real_time(void);

/*
 * Atomic operations
 */
gint g_atomic_int_get(const volatile gint *atomic);
void g_atomic_int_set(volatile gint *atomic, gint newval);
void g_atomic_int_inc(volatile gint *atomic);
gboolean g_atomic_int_dec_and_test(volatile gint *atomic);
gboolean g_atomic_int_compare_and_exchange(volatile gint *atomic, gint oldval, gint newval);
gint g_atomic_int_add(volatile gint *atomic, gint val);
guint g_atomic_int_and(volatile guint *atomic, guint val);
guint g_atomic_int_or(volatile guint *atomic, guint val);
guint g_atomic_int_xor(volatile guint *atomic, guint val);

gpointer g_atomic_pointer_get(const volatile void *atomic);
void g_atomic_pointer_set(volatile void *atomic, gpointer newval);
gboolean g_atomic_pointer_compare_and_exchange(volatile void *atomic, gpointer oldval, gpointer newval);
gssize g_atomic_pointer_add(volatile void *atomic, gssize val);
gsize g_atomic_pointer_and(volatile void *atomic, gsize val);
gsize g_atomic_pointer_or(volatile void *atomic, gsize val);
gsize g_atomic_pointer_xor(volatile void *atomic, gsize val);

/*
 * Once initialization
 */
typedef struct _GOnce GOnce;
struct _GOnce {
    volatile gint status;
    volatile gpointer retval;
};

#define G_ONCE_INIT { 0, NULL }

gpointer g_once_impl(GOnce *once, gpointer (*func)(gpointer), gpointer arg);
gboolean g_once_init_enter(volatile void *location);
void g_once_init_leave(volatile void *location, gsize result);

#define g_once(once, func, arg) g_once_impl(once, func, arg)

/*
 * Thread utilities (using pthreads from libnx)
 */
typedef struct _GThread GThread;
typedef struct _GMutex GMutex;
typedef struct _GRecMutex GRecMutex;
typedef struct _GCond GCond;
typedef struct _GRWLock GRWLock;
typedef struct _GPrivate GPrivate;

/* These will be implemented using pthread equivalents */
GThread* g_thread_new(const gchar *name, gpointer (*func)(gpointer), gpointer data);
GThread* g_thread_try_new(const gchar *name, gpointer (*func)(gpointer), gpointer data, GError **error);
GThread* g_thread_self(void);
gpointer g_thread_join(GThread *thread);
void g_thread_yield(void);
void g_thread_exit(gpointer retval) G_GNUC_NORETURN;

void g_mutex_init(GMutex *mutex);
void g_mutex_clear(GMutex *mutex);
void g_mutex_lock(GMutex *mutex);
gboolean g_mutex_trylock(GMutex *mutex);
void g_mutex_unlock(GMutex *mutex);

void g_rec_mutex_init(GRecMutex *rec_mutex);
void g_rec_mutex_clear(GRecMutex *rec_mutex);
void g_rec_mutex_lock(GRecMutex *rec_mutex);
gboolean g_rec_mutex_trylock(GRecMutex *rec_mutex);
void g_rec_mutex_unlock(GRecMutex *rec_mutex);

void g_cond_init(GCond *cond);
void g_cond_clear(GCond *cond);
void g_cond_wait(GCond *cond, GMutex *mutex);
gboolean g_cond_wait_until(GCond *cond, GMutex *mutex, gint64 end_time);
void g_cond_signal(GCond *cond);
void g_cond_broadcast(GCond *cond);

void g_rw_lock_init(GRWLock *rw_lock);
void g_rw_lock_clear(GRWLock *rw_lock);
void g_rw_lock_writer_lock(GRWLock *rw_lock);
gboolean g_rw_lock_writer_trylock(GRWLock *rw_lock);
void g_rw_lock_writer_unlock(GRWLock *rw_lock);
void g_rw_lock_reader_lock(GRWLock *rw_lock);
gboolean g_rw_lock_reader_trylock(GRWLock *rw_lock);
void g_rw_lock_reader_unlock(GRWLock *rw_lock);

void g_private_init(GPrivate *key, GDestroyNotify notify);
gpointer g_private_get(GPrivate *key);
void g_private_set(GPrivate *key, gpointer value);
void g_private_replace(GPrivate *key, gpointer value);

/*
 * Compatibility with code checking glib version
 */
#define GLIB_MAJOR_VERSION 2
#define GLIB_MINOR_VERSION 66
#define GLIB_MICRO_VERSION 0

#define GLIB_CHECK_VERSION(major, minor, micro) \
    (GLIB_MAJOR_VERSION > (major) || \
     (GLIB_MAJOR_VERSION == (major) && GLIB_MINOR_VERSION > (minor)) || \
     (GLIB_MAJOR_VERSION == (major) && GLIB_MINOR_VERSION == (minor) && \
      GLIB_MICRO_VERSION >= (micro)))

#define GLIB_VERSION_2_26 (G_ENCODE_VERSION(2, 26))
#define GLIB_VERSION_2_28 (G_ENCODE_VERSION(2, 28))
#define GLIB_VERSION_2_30 (G_ENCODE_VERSION(2, 30))
#define GLIB_VERSION_2_32 (G_ENCODE_VERSION(2, 32))
#define GLIB_VERSION_2_34 (G_ENCODE_VERSION(2, 34))
#define GLIB_VERSION_2_36 (G_ENCODE_VERSION(2, 36))
#define GLIB_VERSION_2_38 (G_ENCODE_VERSION(2, 38))
#define GLIB_VERSION_2_40 (G_ENCODE_VERSION(2, 40))
#define GLIB_VERSION_2_42 (G_ENCODE_VERSION(2, 42))
#define GLIB_VERSION_2_44 (G_ENCODE_VERSION(2, 44))
#define GLIB_VERSION_2_46 (G_ENCODE_VERSION(2, 46))
#define GLIB_VERSION_2_48 (G_ENCODE_VERSION(2, 48))
#define GLIB_VERSION_2_50 (G_ENCODE_VERSION(2, 50))
#define GLIB_VERSION_2_52 (G_ENCODE_VERSION(2, 52))
#define GLIB_VERSION_2_54 (G_ENCODE_VERSION(2, 54))
#define GLIB_VERSION_2_56 (G_ENCODE_VERSION(2, 56))
#define GLIB_VERSION_2_58 (G_ENCODE_VERSION(2, 58))
#define GLIB_VERSION_2_60 (G_ENCODE_VERSION(2, 60))
#define GLIB_VERSION_2_62 (G_ENCODE_VERSION(2, 62))
#define GLIB_VERSION_2_64 (G_ENCODE_VERSION(2, 64))
#define GLIB_VERSION_2_66 (G_ENCODE_VERSION(2, 66))
#define GLIB_VERSION_2_68 (G_ENCODE_VERSION(2, 68))

#define G_ENCODE_VERSION(major, minor) ((major) << 16 | (minor) << 8)

/* Version checks that some code uses */
#define GLIB_VERSION_MIN_REQUIRED GLIB_VERSION_2_66
#define GLIB_VERSION_MAX_ALLOWED  GLIB_VERSION_2_66

/*
 * GBytes
 */
typedef struct _GBytes GBytes;

GBytes* g_bytes_new(gconstpointer data, gsize size);
GBytes* g_bytes_new_take(gpointer data, gsize size);
GBytes* g_bytes_new_static(gconstpointer data, gsize size);
GBytes* g_bytes_ref(GBytes *bytes);
void g_bytes_unref(GBytes *bytes);
gconstpointer g_bytes_get_data(GBytes *bytes, gsize *size);
gsize g_bytes_get_size(GBytes *bytes);
guint g_bytes_hash(gconstpointer bytes);
gboolean g_bytes_equal(gconstpointer bytes1, gconstpointer bytes2);
gint g_bytes_compare(gconstpointer bytes1, gconstpointer bytes2);

/*
 * GTree - balanced binary tree
 */
typedef struct _GTree GTree;
typedef gboolean (*GTraverseFunc)(gpointer key, gpointer value, gpointer data);

GTree* g_tree_new(GCompareFunc key_compare_func);
GTree* g_tree_new_with_data(GCompareDataFunc key_compare_func, gpointer key_compare_data);
GTree* g_tree_new_full(GCompareDataFunc key_compare_func, gpointer key_compare_data,
                       GDestroyNotify key_destroy_func, GDestroyNotify value_destroy_func);
GTree* g_tree_ref(GTree *tree);
void g_tree_unref(GTree *tree);
void g_tree_destroy(GTree *tree);
void g_tree_insert(GTree *tree, gpointer key, gpointer value);
void g_tree_replace(GTree *tree, gpointer key, gpointer value);
gboolean g_tree_remove(GTree *tree, gconstpointer key);
gboolean g_tree_steal(GTree *tree, gconstpointer key);
gpointer g_tree_lookup(GTree *tree, gconstpointer key);
gboolean g_tree_lookup_extended(GTree *tree, gconstpointer lookup_key,
                                gpointer *orig_key, gpointer *value);
void g_tree_foreach(GTree *tree, GTraverseFunc func, gpointer user_data);
gint g_tree_height(GTree *tree);
gint g_tree_nnodes(GTree *tree);

/*
 * GDateTime - date/time handling
 */
typedef struct _GDateTime GDateTime;
typedef struct _GTimeZone GTimeZone;

GDateTime* g_date_time_new_now_utc(void);
GDateTime* g_date_time_new_now_local(void);
GDateTime* g_date_time_new(GTimeZone *tz, gint year, gint month, gint day,
                           gint hour, gint minute, gdouble seconds);
GDateTime* g_date_time_ref(GDateTime *datetime);
void g_date_time_unref(GDateTime *datetime);
gchar* g_date_time_format(GDateTime *datetime, const gchar *format);
gchar* g_date_time_format_iso8601(GDateTime *datetime);
gint g_date_time_get_year(GDateTime *datetime);
gint g_date_time_get_month(GDateTime *datetime);
gint g_date_time_get_day_of_month(GDateTime *datetime);
gint g_date_time_get_hour(GDateTime *datetime);
gint g_date_time_get_minute(GDateTime *datetime);
gint g_date_time_get_second(GDateTime *datetime);
gint g_date_time_get_microsecond(GDateTime *datetime);
gint64 g_date_time_to_unix(GDateTime *datetime);

GTimeZone* g_time_zone_new_utc(void);
GTimeZone* g_time_zone_new_local(void);
GTimeZone* g_time_zone_ref(GTimeZone *tz);
void g_time_zone_unref(GTimeZone *tz);

G_DEFINE_AUTOPTR_CLEANUP_FUNC(GDateTime, g_date_time_unref)

/*
 * GPollFD - poll file descriptor structure
 */
typedef struct {
    gint fd;
    gushort events;
    gushort revents;
} GPollFD;

/* Poll event flags */
#define G_IO_IN     0x001
#define G_IO_OUT    0x004
#define G_IO_PRI    0x002
#define G_IO_ERR    0x008
#define G_IO_HUP    0x010
#define G_IO_NVAL   0x020

typedef enum {
    G_IO_STATUS_ERROR,
    G_IO_STATUS_NORMAL,
    G_IO_STATUS_EOF,
    G_IO_STATUS_AGAIN
} GIOStatus;

typedef enum {
    G_IO_FLAG_APPEND = 1 << 0,
    G_IO_FLAG_NONBLOCK = 1 << 1,
    G_IO_FLAG_IS_READABLE = 1 << 2,
    G_IO_FLAG_IS_WRITABLE = 1 << 3,
    G_IO_FLAG_IS_SEEKABLE = 1 << 4,
    G_IO_FLAG_MASK = (1 << 5) - 1,
    G_IO_FLAG_GET_MASK = G_IO_FLAG_MASK,
    G_IO_FLAG_SET_MASK = G_IO_FLAG_APPEND | G_IO_FLAG_NONBLOCK
} GIOFlags;

/*
 * GSource - main context event source (minimal stub)
 */
typedef struct _GMainContext GMainContext;
typedef struct _GMainLoop GMainLoop;
typedef struct _GSource GSource;
typedef struct _GSourceFuncs GSourceFuncs;
typedef struct _GSourceCallbackFuncs GSourceCallbackFuncs;

/* Forward declare GSourceFunc before using it */
typedef gboolean (*GSourceFunc)(gpointer user_data);

/* GSource constants */
#define G_SOURCE_CONTINUE TRUE
#define G_SOURCE_REMOVE FALSE

/* GSource flags */
#define G_SOURCE_CAN_RECURSE  (1 << 0)
#define G_SOURCE_DESTROYED    (1 << 1)

/* GSource priority levels */
#define G_PRIORITY_HIGH            -100
#define G_PRIORITY_DEFAULT          0
#define G_PRIORITY_HIGH_IDLE        100
#define G_PRIORITY_DEFAULT_IDLE     200
#define G_PRIORITY_LOW              300

struct _GSourceCallbackFuncs {
    void (*ref)(gpointer cb_data);
    void (*unref)(gpointer cb_data);
    void (*get)(gpointer cb_data, GSource *source, gpointer *func, gpointer *data);
};

struct _GSource {
    gpointer callback_data;
    GSourceCallbackFuncs *callback_funcs;
    const GSourceFuncs *source_funcs;
    guint ref_count;
    GMainContext *context;
    gint priority;
    guint flags;
    guint source_id;
    GSList *poll_fds;
    GSource *prev;
    GSource *next;
    char *name;
    gpointer priv;
};

struct _GSourceFuncs {
    gboolean (*prepare)(GSource *source, gint *timeout_);
    gboolean (*check)(GSource *source);
    gboolean (*dispatch)(GSource *source, GSourceFunc callback, gpointer user_data);
    void (*finalize)(GSource *source);
    gpointer closure_callback;
    gpointer closure_marshal;
};

GSource* g_source_new(GSourceFuncs *source_funcs, guint struct_size);
void g_source_destroy(GSource *source);
GSource* g_source_ref(GSource *source);
void g_source_unref(GSource *source);
void g_source_set_callback(GSource *source, gpointer func, gpointer data, GDestroyNotify notify);
guint g_source_attach(GSource *source, GMainContext *context);
void g_source_add_poll(GSource *source, GPollFD *fd);
void g_source_remove_poll(GSource *source, GPollFD *fd);
void g_source_set_name(GSource *source, const char *name);
void g_source_set_priority(GSource *source, gint priority);
gboolean g_source_remove(guint tag);

GMainContext* g_main_context_new(void);
GMainContext* g_main_context_ref(GMainContext *context);
void g_main_context_unref(GMainContext *context);
GMainContext* g_main_context_default(void);
gboolean g_main_context_iteration(GMainContext *context, gboolean may_block);
gboolean g_main_context_pending(GMainContext *context);
gboolean g_main_context_acquire(GMainContext *context);
void g_main_context_release(GMainContext *context);
void g_main_context_wakeup(GMainContext *context);

GMainLoop* g_main_loop_new(GMainContext *context, gboolean is_running);
void g_main_loop_run(GMainLoop *loop);
void g_main_loop_quit(GMainLoop *loop);
gboolean g_main_loop_is_running(GMainLoop *loop);
GMainContext* g_main_loop_get_context(GMainLoop *loop);
GMainLoop* g_main_loop_ref(GMainLoop *loop);
void g_main_loop_unref(GMainLoop *loop);

void g_source_set_can_recurse(GSource *source, gboolean can_recurse);
gboolean g_source_is_destroyed(GSource *source);

GSource* g_timeout_source_new(guint interval);
gint g_poll(GPollFD *fds, guint nfds, gint timeout);

guint g_timeout_add(guint interval, GSourceFunc function, gpointer data);
guint g_timeout_add_full(gint priority, guint interval, GSourceFunc function, gpointer data, GDestroyNotify notify);
guint g_idle_add(GSourceFunc function, gpointer data);
guint g_idle_add_full(gint priority, GSourceFunc function, gpointer data, GDestroyNotify notify);

/*
 * GIOChannel - I/O channel abstraction (stub)
 */
typedef struct _GIOChannel GIOChannel;

GIOChannel* g_io_channel_unix_new(int fd);
void g_io_channel_unref(GIOChannel *channel);
GIOStatus g_io_channel_set_encoding(GIOChannel *channel, const gchar *encoding, GError **error);
void g_io_channel_set_buffered(GIOChannel *channel, gboolean buffered);

/*
 * GLib Unix functions (glib-unix.h)
 */
gboolean g_unix_open_pipe(gint *fds, gint flags, GError **error);
gboolean g_unix_set_fd_nonblocking(gint fd, gboolean nonblock, GError **error);
guint g_unix_fd_add(gint fd, gint condition, gboolean (*function)(gint, gint, gpointer), gpointer user_data);

/* FD_CLOEXEC may not be defined everywhere */
#ifndef FD_CLOEXEC
#define FD_CLOEXEC 1
#endif

#ifdef __cplusplus
}
#endif

#endif /* SWITCH_GLIB_COMPAT_H */

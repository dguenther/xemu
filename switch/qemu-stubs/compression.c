/*
 * QEMU Stubs for Nintendo Switch - Compression (zlib, xxHash)
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"
#include "common.h"

/*
 * zlib stubs (compression disabled)
 */
typedef struct z_stream_s z_stream;

int deflateInit2_(z_stream *strm, int level, int method, int windowBits,
                  int memLevel, int strategy, const char *version, int stream_size)
{
    (void)strm;
    (void)level;
    (void)method;
    (void)windowBits;
    (void)memLevel;
    (void)strategy;
    (void)version;
    (void)stream_size;
    return -1;
}

int deflate(z_stream *strm, int flush)
{
    (void)strm;
    (void)flush;
    return -1;
}

int deflateEnd(z_stream *strm)
{
    (void)strm;
    return 0;
}

int inflateInit2_(z_stream *strm, int windowBits, const char *version, int stream_size)
{
    (void)strm;
    (void)windowBits;
    (void)version;
    (void)stream_size;
    return -1;
}

int inflate(z_stream *strm, int flush)
{
    (void)strm;
    (void)flush;
    return -1;
}

int inflateEnd(z_stream *strm)
{
    (void)strm;
    return 0;
}

/*
 * xxHash stub
 */
uint64_t XXH3_64bits(const void *data, size_t len)
{
    (void)data;
    (void)len;
    return 0;
}

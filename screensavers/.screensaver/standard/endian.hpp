/*******************************************************************************
 * Implementation of various size byte-swap operations as functions/macros.
 * Authored by Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

#ifndef ENDIAN_HPP__ /*///////////////////////////////////////////////////////*/
#define ENDIAN_HPP__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define LIL_ENDIAN 0x11223344
#define BIG_ENDIAN 0x44332211

// We know that all Windows machines will be running as lil endian.
#if defined(PLATFORM_WIN32)
#define BYTEORDER LIL_ENDIAN
#endif

/* -------------------------------------------------------------------------- */

#if defined(COMPILER_MSVC)
#define swap16(x) _byteswap_ushort(x)
#else
FILDEF u16 swap16 (u16 x)
{
    return CAST(u16, ((x << 8) | (x >> 8)));
}
#endif // defined(COMPILER_MSVC)

#if defined(COMPILER_MSVC)
#define swap32(x) _byteswap_ulong(x)
#else
FILDEF u32 swap32 (u32 x)
{
    return CAST(u32, (((x<<24))            |
                      ((x<< 8)&0x00FF0000) |
                      ((x>> 8)&0x0000FF00) |
                      ((x>>24))));
}
#endif // defined(COMPILER_MSVC)

#if defined(COMPILER_MSVC)
#define swap64(x) _byteswap_uint64(x)
#else
FILDEF u64 swap64 (u64 x)
{
    u32 lo = CAST(u32, (x & 0xFFFFFFFF));
    x >>= 32;
    u32 hi = CAST(u32, (x & 0xFFFFFFFF));
    x   = swap32(lo);
    x <<= 32;
    x  |= swap32(hi);
    return x;
}
#endif // defined(COMPILER_MSVC)

/* -------------------------------------------------------------------------- */

// Swap to a specific byteorder regardless of machine endianness.
#if (BYTEORDER == LIL_ENDIAN)
#define lswap16(x)       (x)
#define lswap32(x)       (x)
#define lswap64(x)       (x)
#define bswap16(x) swap16(x)
#define bswap32(x) swap32(x)
#define bswap64(x) swap64(x)
#else
#define lswap16(x) swap16(x)
#define lswap32(x) swap32(x)
#define lswap64(x) swap64(x)
#define bswap16(x)       (x)
#define bswap32(x)       (x)
#define bswap64(x)       (x)
#endif

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* ENDIAN_HPP__ ///////////////////////////////////////////////////////*/

/*******************************************************************************
 *
 * Copyright (c) 2020 Joshua Robertson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
*******************************************************************************/

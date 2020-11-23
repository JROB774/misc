/*******************************************************************************
 * Defines used throughout the standard library codebase.
 * Authored by Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

#ifndef STANDARD_H__ /*///////////////////////////////////////////////////////*/
#define STANDARD_H__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define STRINGIFY( x) STRINGIFY1(x)
#define STRINGIFY1(x) #x

#define JOIN( a, b) JOIN2(a, b)
#define JOIN2(a, b) JOIN1(a, b)
#define JOIN1(a, b) a##b

/* -------------------------------------------------------------------------- */

#define PERSISTENT static
#define INTERNAL   static
#define GLOBAL     static
#define SHARED     static

/* -------------------------------------------------------------------------- */

#if   defined(COMPILER_HAS_CPLUSPLUS)
#define INLINE inline
#else
#if   defined(COMPILER_CLANG)
#define INLINE   inline
#elif defined(COMPILER_GCC)
#define INLINE __inline__
#elif defined(COMPILER_MSVC)
#define INLINE __inline
#endif
#endif

#if    defined(COMPILER_CLANG)
#define FORCE_INLINE INLINE __attribute__((always_inline))
#elif (defined(COMPILER_GCC) && (COMPILER_VERNUM >= 301))
#define FORCE_INLINE INLINE __attribute__((always_inline))
#elif  defined(COMPILER_MSVC)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE INLINE
#endif

/* -------------------------------------------------------------------------- */

#define STDDEF INTERNAL
#define INLDEF INTERNAL INLINE
#define FILDEF INTERNAL FORCE_INLINE

/* -------------------------------------------------------------------------- */

#if defined(COMPILER_HAS_CPLUSPLUS)
#define SCAST(t, x) static_cast     <t>(x)
#define DCAST(t, x) dynamic_cast    <t>(x)
#define RCAST(t, x) reinterpret_cast<t>(x)
#define CCAST(t, x) const_cast      <t>(x)
#else
#define SCAST(t, x) ((t)(x))
#define DCAST(t, x) ((t)(x))
#define RCAST(t, x) ((t)(x))
#define CCAST(t, x) ((t)(x))
#endif

#define CAST(t, x) ((t)(x))

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* STANDARD_H__ ///////////////////////////////////////////////////////*/

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

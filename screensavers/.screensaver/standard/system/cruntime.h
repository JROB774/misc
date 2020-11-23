/*******************************************************************************
 * Implementation of required low-level C Runtime features and functionality.
 * Authored By Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

#ifndef CRUNTIME_H__ /*///////////////////////////////////////////////////////*/
#define CRUNTIME_H__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

// Undefine some things that may already be defined so we can redefine our own.
#ifdef EXTERN_C_START
#undef EXTERN_C_START
#endif
#ifdef EXTERN_C_END
#undef EXTERN_C_END
#endif
#ifdef EXTERN_C
#undef EXTERN_C
#endif

#ifdef COMPILER_HAS_CPLUSPLUS
#define EXTERN_C_START extern "C" {
#else
#define EXTERN_C_START
#endif

#ifdef COMPILER_HAS_CPLUSPLUS
#define EXTERN_C_END }
#else
#define EXTERN_C_END
#endif

#ifdef COMPILER_HAS_CPLUSPLUS
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#ifdef COMPILER_MSVC
#define CRTCALL __cdecl
#else
#define CRTCALL
#endif

/* -------------------------------------------------------------------------- */

#include <system/cruntime/crt_string.c>

/* -------------------------------------------------------------------------- */

// These headers are safe to include as they do not involve any C runtime code.
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include  <float.h>

/* -------------------------------------------------------------------------- */

EXTERN_C_START /***************************************************************/

/* -------------------------------------------------------------------------- */

typedef  uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef   int8_t  s8;
typedef  int16_t s16;
typedef  int32_t s32;
typedef  int64_t s64;

/* -------------------------------------------------------------------------- */

// If in C then the boolean type does not exist so we can just define our own.
#ifndef COMPILER_HAS_CPLUSPLUS
#define false 0
#define true  1
typedef int bool;
#endif

/* -------------------------------------------------------------------------- */

// Useless symbol states we are using floating-point values.
#ifndef USE_CRT
int _fltused;
#endif

/* -------------------------------------------------------------------------- */

#ifdef ARCHITECTURE_X86 /******************************************************/

#ifndef USE_CRT /**************************************************************/

#include <system/cruntime/asm86/alldiv.c>
#include <system/cruntime/asm86/alldvrm.c>
#include <system/cruntime/asm86/allmul.c>
#include <system/cruntime/asm86/allrem.c>
#include <system/cruntime/asm86/allshl.c>
#include <system/cruntime/asm86/allshr.c>
#include <system/cruntime/asm86/aulldiv.c>
#include <system/cruntime/asm86/aulldvrm.c>
#include <system/cruntime/asm86/aullrem.c>
#include <system/cruntime/asm86/aullshr.c>
#include <system/cruntime/asm86/ftol2.c>

#endif /* USE_CRT *************************************************************/

#endif /* ARCHITECTURE_X86 ****************************************************/

/* -------------------------------------------------------------------------- */

EXTERN_C_END /*****************************************************************/

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* CRUNTIME_H__ ///////////////////////////////////////////////////////*/

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

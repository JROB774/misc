/*******************************************************************************
 * Implementation of portions of the CRT file <math.h> that we desire.
 * Authored By Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//
// THE FUNCTIONS IMPLEMENTED BY THIS FILE ARE AS FOLLOWS:
//
// double copysign  (double x)
// double cos       (double x)
// double fabs      (double x)
// double scalbln   (double x, long n)
// double scalbn    (double x, int  n)
// double sin       (double x)
// double tan       (double x)
// float  copysignf (float  x)
// float  cosf      (float  x)
// float  fabsf     (float  x)
// float  scalblnf  (float  x, long n)
// float  scalbnf   (float  x, int  n)
// float  sinf      (float  x)
// float  tanf      (float  x)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CRT_MATH_C__ /*///////////////////////////////////////////////////////*/
#define CRT_MATH_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#ifdef USE_CRT
#include <math.h>
#endif

/* -------------------------------------------------------------------------- */

#ifndef USE_CRT /**************************************************************/

/* -------------------------------------------------------------------------- */

EXTERN_C_START /***************************************************************/

/* -------------------------------------------------------------------------- */

#if defined(COMPILER_MSVC)
#pragma warning(push, 0)
#endif // COMPILER_MSVC

/* -------------------------------------------------------------------------- */

// We use the libm library to implement low-level mathetmatical operations.

#include <system/cruntime/libm/math_private.h>
#include <system/cruntime/libm/s_copysign.c>
#include <system/cruntime/libm/s_scalbn.c>
#include <system/cruntime/libm/s_floor.c>
#include <system/cruntime/libm/k_rem_pio2.c>
#include <system/cruntime/libm/s_fabs.c>
#include <system/cruntime/libm/k_cos.c>
#include <system/cruntime/libm/k_sin.c>
#include <system/cruntime/libm/k_tan.c>
#include <system/cruntime/libm/e_rem_pio2.c>
#include <system/cruntime/libm/s_cos.c>
#include <system/cruntime/libm/s_sin.c>
#include <system/cruntime/libm/s_tan.c>

/* -------------------------------------------------------------------------- */

#define copysign(  x    ) impl__copysign  ( x    )
#define cos(       x    ) impl__cos       ( x    )
#define fabs(      x    ) impl__fabs      ( x    )
#define scalbln(   x, n ) impl__scalbln   ( x, n )
#define scalbn(    x, n ) impl__scalbn    ( x, n )
#define sin(       x    ) impl__sin       ( x    )
#define tan(       x    ) impl__tan       ( x    )
#define copysignf( x    ) impl__copysignf ( x    )
#define cosf(      x    ) impl__cosf      ( x    )
#define fabsf(     x    ) impl__fabsf     ( x    )
#define scalblnf(  x, n ) impl__scalblnf  ( x, n )
#define scalbnf(   x, n ) impl__scalbnf   ( x, n )
#define sinf(      x    ) impl__sinf      ( x    )
#define tanf(      x    ) impl__tanf      ( x    )

/* -------------------------------------------------------------------------- */

#if defined(COMPILER_MSVC)
#pragma warning(pop)
#endif // COMPILER_MSVC

/* -------------------------------------------------------------------------- */

EXTERN_C_END /*****************************************************************/

/* -------------------------------------------------------------------------- */

#endif /* !USE_CRT ************************************************************/

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* CRT_MATH_C__ ///////////////////////////////////////////////////////*/

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

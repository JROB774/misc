/*******************************************************************************
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 *
*******************************************************************************/

#ifndef LIBM_PRIVATE_H__ /*****************************************************/
#define LIBM_PRIVATE_H__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

#include <endian.hpp>

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

// @Temporary: Just actually change the names and remove all of this junk???
// Define this stuff to get the libm codebase working correctly for us.

typedef u32 u_int32_t;

/* -------------------------------------------------------------------------- */

////////////////////////////////////////////////////////////////////////////////
//
// The original fdlibm code used statements like:
//
//     n0 = ((*(int*)&one)>>29)^1;     * index of high word *
//    ix0 = *(n0+(int*)&x);            * high word of x     *
//    ix1 = *((1-n0)+(int*)&x);        * low word of x      *
//
// to dig two 32 bit words out of the 64 bit IEEE floating point
// value.  That is non-ANSI, and, moreover, the gcc instruction
// scheduler gets it wrong.  We instead use the following macros.
// Unlike the original code, we determine the endianness at compile
// time, not at run time; I don't see much benefit to selecting
// endianness at run time.
//
////////////////////////////////////////////////////////////////////////////////

// A union which permits us to convert between a double and two 32 bit ints.

#if (BYTEORDER == BIG_ENDIAN) /************************************************/

typedef union
{
    double value;
    struct
    {
        u_int32_t msw;
        u_int32_t lsw;
    } parts;
} ieee_double_shape_type;

#else

typedef union
{
    double value;
    struct
    {
        u_int32_t lsw;
        u_int32_t msw;
    } parts;
} ieee_double_shape_type;

#endif /* (BYTEORDER == BIG_ENDIAN) *******************************************/

/* -------------------------------------------------------------------------- */

// Get two 32 bit ints from a double.
#define EXTRACT_WORDS(ix0, ix1, d) \
do                                 \
{                                  \
ieee_double_shape_type ew_u;       \
ew_u.value = (d);                  \
(ix0) = ew_u.parts.msw;            \
(ix1) = ew_u.parts.lsw;            \
}                                  \
while (0,0)

// Get the more significant 32 bit int from a double.
#define GET_HIGH_WORD(i, d)        \
do                                 \
{                                  \
ieee_double_shape_type gh_u;       \
gh_u.value = (d);                  \
(i) = gh_u.parts.msw;              \
}                                  \
while (0,0)

// Get the less significant 32 bit int from a double.
#define GET_LOW_WORD(i, d)         \
do                                 \
{                                  \
ieee_double_shape_type gl_u;       \
gl_u.value = (d);                  \
(i) = gl_u.parts.lsw;              \
}                                  \
while (0,0)

// Set a double from two 32 bit ints.
#define INSERT_WORDS(d, ix0, ix1)  \
do                                 \
{                                  \
ieee_double_shape_type iw_u;       \
iw_u.parts.msw = (ix0);            \
iw_u.parts.lsw = (ix1);            \
(d) = iw_u.value;                  \
}                                  \
while (0,0)

// Set the more significant 32 bits of a double from an int.
#define SET_HIGH_WORD(d, v)        \
do                                 \
{                                  \
ieee_double_shape_type sh_u;       \
sh_u.value = (d);                  \
sh_u.parts.msw = (v);              \
(d) = sh_u.value;                  \
}                                  \
while (0,0)

// Set the less significant 32 bits of a double from an int.
#define SET_LOW_WORD(d, v)         \
do                                 \
{                                  \
ieee_double_shape_type sl_u;       \
sl_u.value = (d);                  \
sl_u.parts.lsw = (v);              \
(d) = sl_u.value;                  \
}                                  \
while (0,0)

/* -------------------------------------------------------------------------- */

// A union which permits us to convert between a float and a 32 bit int.

typedef union
{
    float value;
    u_int32_t word;
} ieee_float_shape_type;

/* -------------------------------------------------------------------------- */

// Get a 32 bit int from a float.
#define GET_FLOAT_WORD(i, d)       \
do                                 \
{                                  \
ieee_float_shape_type gf_u;        \
gf_u.value = (d);                  \
(i) = gf_u.word;                   \
}                                  \
while (0,0)

// Set a float from a 32 bit int.
#define SET_FLOAT_WORD(d, i)       \
do                                 \
{                                  \
  ieee_float_shape_type sf_u;      \
  sf_u.word = (i);                 \
  (d) = sf_u.value;                \
}                                  \
while (0,0)

/* -------------------------------------------------------------------------- */

// IEEE style elementary functions.

// STDDEF double __ieee754_sqrt      (double)          attribute_hidden;
// STDDEF double __ieee754_acos      (double)          attribute_hidden;
// STDDEF double __ieee754_acosh     (double)          attribute_hidden;
// STDDEF double __ieee754_log       (double)          attribute_hidden;
// STDDEF double __ieee754_log2      (double)          attribute_hidden;
// STDDEF double __ieee754_atanh     (double)          attribute_hidden;
// STDDEF double __ieee754_asin      (double)          attribute_hidden;
// STDDEF double __ieee754_atan2     (double, double)  attribute_hidden;
// STDDEF double __ieee754_exp       (double)          attribute_hidden;
// STDDEF double __ieee754_exp10     (double)          attribute_hidden;
// STDDEF double __ieee754_cosh      (double)          attribute_hidden;
// STDDEF double __ieee754_fmod      (double, double)  attribute_hidden;
// STDDEF double __ieee754_pow       (double, double)  attribute_hidden;
// STDDEF double __ieee754_lgamma_r  (double, int*)    attribute_hidden;
// STDDEF double __ieee754_lgamma    (double)          attribute_hidden;
// STDDEF double __ieee754_log10     (double)          attribute_hidden;
// STDDEF double __ieee754_sinh      (double)          attribute_hidden;
// STDDEF double __ieee754_hypot     (double, double)  attribute_hidden;
// STDDEF double __ieee754_j0        (double)          attribute_hidden;
// STDDEF double __ieee754_j1        (double)          attribute_hidden;
// STDDEF double __ieee754_y0        (double)          attribute_hidden;
// STDDEF double __ieee754_y1        (double)          attribute_hidden;
// STDDEF double __ieee754_jn        (int, double)     attribute_hidden;
// STDDEF double __ieee754_yn        (int, double)     attribute_hidden;
// STDDEF double __ieee754_remainder (double, double)  attribute_hidden;
// STDDEF double __ieee754_scalb     (double, double)  attribute_hidden;

STDDEF int    __ieee754_rem_pio2  (double, double*);

// fdlibm kernel functions.

STDDEF double __kernel_sin        (double, double, int);
STDDEF double __kernel_cos        (double, double);
STDDEF double __kernel_tan        (double, double, int);
STDDEF int    __kernel_rem_pio2   (double*, double*, int, int, int, const int*);

/* -------------------------------------------------------------------------- */

////////////////////////////////////////////////////////////////////////////////
//
// math_opt_barrier(x): safely load x, even if it was manipulated
// by non-floationg point operations. This macro returns the value of x.
// This ensures compiler does not (ab)use its knowledge about x value
// and don't optimize future operations. Example:
//
//     float x;
//     SET_FLOAT_WORD(x, 0x80000001); // sets a bit pattern
//     y = math_opt_barrier(x);       // compiler, do not cheat!
//     y = y * y;                     // compiler can't optimize
//
// math_force_eval(x): force expression x to be evaluated.
// Useful if otherwise compiler may eliminate the expression
// as unused. This macro returns no value.
// Example:
//
//     "void fn(float f) { f = f * f; }"
// versus
//     "void fn(float f) { f = f * f; math_force_eval(f); }"
//
// Currently, math_force_eval(x) stores x into
// a floating point register or memory *of the appropriate size*.
// There is no guarantee this will not change.
//
////////////////////////////////////////////////////////////////////////////////

#if defined(__i386__)
#define math_opt_barrier(x)                 \
({                                          \
decltype(x) __x = (x);                      \
/* "t": load x into top-of-stack fpreg */   \
__asm__ ("" : "=t" (__x) : "0" (__x));      \
__x;                                        \
})
#define math_force_eval(x)                  \
do                                          \
{                                           \
decltype(x) __x = (x);                      \
if (sizeof(__x) <= sizeof(double))          \
  /* "m": store x into a memory location */ \
  __asm__ __volatile__ ("" : : "m" (__x));  \
else /* long double */                      \
  /* "f": load x into (any) fpreg */        \
  __asm__ __volatile__ ("" : : "f" (__x));  \
}                                           \
while (0,0)
#endif // __i386__

#if defined(__x86_64__)
#define math_opt_barrier(x)                 \
({                                          \
decltype(x) __x = (x);                      \
if (sizeof(__x) <= sizeof(double))          \
  /* "x": load into XMM SSE register */     \
  __asm__ ("" : "=x" (__x) : "0" (__x));    \
else /* long double */                      \
  /* "t": load x into top-of-stack fpreg */ \
  __asm__ ("" : "=t" (__x) : "0" (__x));    \
__x;                                        \
})
#define math_force_eval(x)                  \
do                                          \
{                                           \
decltype(x) __x = (x);                      \
if (sizeof(__x) <= sizeof(double))          \
  /* "x": load into XMM SSE register */     \
  __asm__ __volatile__ ("" : : "x" (__x));  \
else /* long double */                      \
  /* "f": load x into (any) fpreg */        \
  __asm__ __volatile__ ("" : : "f" (__x));  \
}                                           \
while (0,0)
#endif // __x86_64__

// Default implementations force store to a memory location.

#ifndef math_opt_barrier
#define math_opt_barrier(x)                 \
({                                          \
decltype(x) __x = (x);                      \
__asm__ ("" : "+m" (__x));                  \
__x;                                        \
})
#endif // math_opt_barrier

#ifndef math_force_eval
#define math_force_eval(x)                  \
do                                          \
{                                           \
decltype(x) __x = (x);                      \
__asm__ __volatile__ ("" : : "m" (__x));    \
}                                           \
while (0,0)
#endif // math_force_eval

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* LIBM_PRIVATE_H__ ///////////////////////////////////////////////////*/

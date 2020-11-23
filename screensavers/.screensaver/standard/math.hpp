/*******************************************************************************
 * Implementation of various mathematical functions and constants.
 * Authored By Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

#ifndef MATH_HPP__ /*/////////////////////////////////////////////////////////*/
#define MATH_HPP__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

GLOBAL constexpr double M_E         = 2.71828182845904523536;
GLOBAL constexpr double M_LOG2E     = 1.44269504088896340736;
GLOBAL constexpr double M_LOG10E    = 0.434294481903251827651;
GLOBAL constexpr double M_LN2       = 0.693147180559945309417;
GLOBAL constexpr double M_LN10      = 2.30258509299404568402;
GLOBAL constexpr double M_PI        = 3.14159265358979323846;
GLOBAL constexpr double M_PI_2      = 1.57079632679489661923;
GLOBAL constexpr double M_PI_4      = 0.785398163397448309616;
GLOBAL constexpr double M_1_PI      = 0.318309886183790671538;
GLOBAL constexpr double M_2_PI      = 0.636619772367581343076;
GLOBAL constexpr double M_2_SQRTPI  = 1.12837916709551257390;
GLOBAL constexpr double M_SQRT2     = 1.41421356237309504880;
GLOBAL constexpr double M_SQRT1_2   = 0.707106781186547524401;
GLOBAL constexpr double M_TAU       = 6.28318530717958647692;

GLOBAL constexpr float  M_Ef        = 2.718281828459f;
GLOBAL constexpr float  M_LOG2Ef    = 1.442695040889f;
GLOBAL constexpr float  M_LOG10Ef   = 0.4342944819033f;
GLOBAL constexpr float  M_LN2f      = 0.6931471805599f;
GLOBAL constexpr float  M_LN10f     = 2.302585092994f;
GLOBAL constexpr float  M_PIf       = 3.141592653590f;
GLOBAL constexpr float  M_PI_2f     = 1.570796326795f;
GLOBAL constexpr float  M_PI_4f     = 0.7853981633974f;
GLOBAL constexpr float  M_1_PIf     = 0.3183098861838f;
GLOBAL constexpr float  M_2_PIf     = 0.6366197723676f;
GLOBAL constexpr float  M_2_SQRTPIf = 1.128379167095f;
GLOBAL constexpr float  M_SQRT2f    = 1.414213562373f;
GLOBAL constexpr float  M_SQRT1_2f  = 0.7071067811865f;
GLOBAL constexpr float  M_TAUf      = 6.283185307180f;

/* -------------------------------------------------------------------------- */

// Implementation of various CRT mathematical operations/functions.
#include <system/cruntime/crt_math.c>

/* -------------------------------------------------------------------------- */

template<typename T>
struct vec2_
{
    union { T x, r, s; };
    union { T y, g, t; };
};

template<typename T>
struct vec3_
{
    union { T x, r, s; };
    union { T y, g, t; };
    union { T z, b, p; };
};

template<typename T>
struct vec4_
{
    union { T x, r, s; };
    union { T y, g, t; };
    union { T z, b, p; };
    union { T w, a, q; };
};

/* -------------------------------------------------------------------------- */

typedef vec2_<bool        > bvec2;
typedef vec2_<int         > ivec2;
typedef vec2_<unsigned int> uvec2;
typedef vec2_<float       >  vec2;
typedef vec2_<double      > dvec2;
typedef vec3_<bool        > bvec3;
typedef vec3_<int         > ivec3;
typedef vec3_<unsigned int> uvec3;
typedef vec3_<float       >  vec3;
typedef vec3_<double      > dvec3;
typedef vec4_<bool        > bvec4;
typedef vec4_<int         > ivec4;
typedef vec4_<unsigned int> uvec4;
typedef vec4_<float       >  vec4;
typedef vec4_<double      > dvec4;

/* -------------------------------------------------------------------------- */

template<typename T, size_t M>
struct mat_col
{
    T row[M];

          T&             operator[] (size_t idx)       { return row[idx]; }
    const T&             operator[] (size_t idx) const { return row[idx]; }
};

template<typename T, size_t N, size_t M>
struct mat
{
    mat_col<T, M> col[N];

          mat_col<T, M>& operator[] (size_t idx)       { return col[idx]; }
    const mat_col<T, M>& operator[] (size_t idx) const { return col[idx]; }
};

/* -------------------------------------------------------------------------- */

typedef mat<float, 2, 2> mat2;
typedef mat<float, 2, 2> mat2x2;
typedef mat<float, 2, 3> mat2x3;
typedef mat<float, 2, 4> mat2x4;
typedef mat<float, 3, 3> mat3;
typedef mat<float, 3, 2> mat3x2;
typedef mat<float, 3, 3> mat3x3;
typedef mat<float, 3, 4> mat3x4;
typedef mat<float, 4, 4> mat4;
typedef mat<float, 4, 2> mat4x2;
typedef mat<float, 4, 3> mat4x3;
typedef mat<float, 4, 4> mat4x4;

/* -------------------------------------------------------------------------- */

FILDEF constexpr double torad  (double deg) { return deg * M_PI   / 180.0 ; }
FILDEF constexpr float  toradf (float  deg) { return deg * M_PIf  / 180.0f; }
FILDEF constexpr double todeg  (double rad) { return rad * 180.0  / M_PI  ; }
FILDEF constexpr float  todegf (float  rad) { return rad * 180.0f / M_PIf ; }

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* MATH_HPP__ /////////////////////////////////////////////////////////*/

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

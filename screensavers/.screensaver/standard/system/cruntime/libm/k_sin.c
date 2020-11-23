/*******************************************************************************
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 *
*******************************************************************************/

#ifndef K_SIN_C__ /*//////////////////////////////////////////////////////////*/
#define K_SIN_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define half  5.00000000000000000000e-01 /* 0x3FE00000, 0x00000000 */
#define S1   -1.66666666666666324348e-01 /* 0xBFC55555, 0x55555549 */
#define S2    8.33333333332248946124e-03 /* 0x3F811111, 0x1110F8A6 */
#define S3   -1.98412698298579493134e-04 /* 0xBF2A01A0, 0x19C161D5 */
#define S4    2.75573137070700676789e-06 /* 0x3EC71DE3, 0x57B1FE7D */
#define S5   -2.50507602534068634195e-08 /* 0xBE5AE5E6, 0x8A2B9CEB */
#define S6    1.58969099521155010221e-10 /* 0x3DE5D93A, 0x5ACFD57C */

/* -------------------------------------------------------------------------- */

STDDEF double __kernel_sin (double x, double y, int iy)
{
    double z,r,v;
    int32_t ix;
    GET_HIGH_WORD(ix,x);
    ix &= 0x7fffffff;
    if (ix<0x3e400000)
    {
        if ((int)x==0) return x;
    }
    z = x*x;
    v = z*x;
    r = S2+z*(S3+z*(S4+z*(S5+z*S6)));
    if (iy==0) return x+v*(S1+z*r);
    else       return x-((z*(half*y-v*r)-y)-v*S1);
}

/* -------------------------------------------------------------------------- */

#undef half
#undef S1
#undef S2
#undef S3
#undef S4
#undef S5
#undef S6

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* K_SIN_C__ //////////////////////////////////////////////////////////*/

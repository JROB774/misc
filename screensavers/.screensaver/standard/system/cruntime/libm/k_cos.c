/*******************************************************************************
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 *
*******************************************************************************/

#ifndef K_COS_C__ /*//////////////////////////////////////////////////////////*/
#define K_COS_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define one  1.00000000000000000000e+00 /* 0x3FF00000, 0x00000000 */
#define C1   4.16666666666666019037e-02 /* 0x3FA55555, 0x5555554C */
#define C2  -1.38888888888741095749e-03 /* 0xBF56C16C, 0x16C15177 */
#define C3   2.48015872894767294178e-05 /* 0x3EFA01A0, 0x19CB1590 */
#define C4  -2.75573143513906633035e-07 /* 0xBE927E4F, 0x809C52AD */
#define C5   2.08757232129817482790e-09 /* 0x3E21EE9E, 0xBDB4B1C4 */
#define C6  -1.13596475577881948265e-11 /* 0xBDA8FAE9, 0xBE8838D4 */

/* -------------------------------------------------------------------------- */

STDDEF double __kernel_cos (double x, double y)
{
    double a,hz,z,r,qx;
    int32_t ix;
    GET_HIGH_WORD(ix,x);
    ix &= 0x7fffffff;
    if (ix<0x3e400000)
    {
        if (((int)x)==0) return one;
    }
    z  = x*x;
    r  = z*(C1+z*(C2+z*(C3+z*(C4+z*(C5+z*C6)))));
    if (ix < 0x3FD33333)
    {
        return one - (0.5*z - (z*r - x*y));
    }
    else
    {
        if (ix > 0x3fe90000)
        {
            qx = 0.28125;
        }
        else
        {
            INSERT_WORDS(qx,ix-0x00200000,0);
        }
        hz = 0.5*z-qx;
        a  = one-qx;
        return a - (hz - (z*r-x*y));
    }
}

/* -------------------------------------------------------------------------- */

#undef one
#undef C1
#undef C2
#undef C3
#undef C4
#undef C5
#undef C6

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* K_COS_C__ //////////////////////////////////////////////////////////*/

/*******************************************************************************
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 *
*******************************************************************************/

#ifndef S_SIN_C__ /*//////////////////////////////////////////////////////////*/
#define S_SIN_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

STDDEF double impl__sin (double x)
{
    double y[2],z=0.0;
    int32_t n, ix;

    GET_HIGH_WORD(ix,x);

    ix &= 0x7fffffff;
    if (ix <= 0x3fe921fb)
    {
    	return __kernel_sin(x,z,0);
    }
    else if (ix>=0x7ff00000)
    {
    	return x-x;
    }
    else
    {
        n = __ieee754_rem_pio2(x,y);
        switch (n&3)
        {
        case 0 : return  __kernel_sin(y[0],y[1],1);
        case 1 : return  __kernel_cos(y[0],y[1]);
        case 2 : return -__kernel_sin(y[0],y[1],1);
        default: return -__kernel_cos(y[0],y[1]);
        }
    }
}

FILDEF float impl__sinf (float x)
{
    return (float)impl__sin((double)x);
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* S_SIN_C__ //////////////////////////////////////////////////////////*/

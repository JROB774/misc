/*******************************************************************************
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 *
*******************************************************************************/

#ifndef S_COPYSIGN_C__ /*/////////////////////////////////////////////////////*/
#define S_COPYSIGN_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

STDDEF double impl__copysign (double x, double y)
{
    u_int32_t hx,hy;
    GET_HIGH_WORD(hx,x);
    GET_HIGH_WORD(hy,y);
    SET_HIGH_WORD(x,(hx&0x7fffffff)|(hy&0x80000000));
    return x;
}

FILDEF float impl__copysignf (float x, float y)
{
    return (float)impl__copysign((double)x, (double)y);
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* S_COPYSIGN_C__ /////////////////////////////////////////////////////*/

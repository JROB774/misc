/*******************************************************************************
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 *
*******************************************************************************/

#ifndef S_FABS_C__ /*/////////////////////////////////////////////////////////*/
#define S_FABS_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

STDDEF double impl__fabs (double x)
{
    u_int32_t high;
    GET_HIGH_WORD(high,x);
    SET_HIGH_WORD(x,high&0x7fffffff);
    return x;
}

FILDEF float impl__fabsf (float x)
{
    return (float)impl__fabs((double)x);
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* S_FABS_C__ /////////////////////////////////////////////////////////*/

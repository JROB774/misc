/*******************************************************************************
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 *
*******************************************************************************/

#ifndef S_SCALBN_C__ /*///////////////////////////////////////////////////////*/
#define S_SCALBN_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define two54  1.80143985094819840000e+16 /* 0x43500000, 0x00000000 */
#define twom54 5.55111512312578270212e-17 /* 0x3C900000, 0x00000000 */
#define huge   1.0e+300
#define tiny   1.0e-300

/* -------------------------------------------------------------------------- */

STDDEF double impl__scalbln (double x, long n)
{
    int32_t k, hx, lx;

    EXTRACT_WORDS(hx, lx, x);
    k = (hx & 0x7ff00000) >> 20;
    if (k == 0)
    {
        if ((lx | (hx & 0x7fffffff)) == 0)
        {
            return x;
        }
        x *= two54;
        GET_HIGH_WORD(hx, x);
        k = ((hx & 0x7ff00000) >> 20) - 54;
    }
    if (k == 0x7ff)
    {
        return x + x;
    }
    k = k + n;
    if (k > 0x7fe)
    {
        return huge * impl__copysign(huge, x);
    }
    if (n < -50000)
    {
        return tiny * impl__copysign(tiny, x);
    }
    if (k > 0)
    {
        SET_HIGH_WORD(x, (hx & 0x800fffff) | (k << 20));
        return x;
    }
    if (k <= -54)
    {
        if (n > 50000)
        {
            return huge * impl__copysign(huge, x);
        }
        return tiny * impl__copysign(tiny, x);
    }
    k += 54;
    SET_HIGH_WORD(x, (hx & 0x800fffff) | (k << 20));
    return x * twom54;
}

FILDEF float impl__scalblnf (float x, long n)
{
    return (float)impl__scalbln((double)x, n);
}

STDDEF double impl__scalbn (double x, int n)
{
    return impl__scalbln(x, n);
}

FILDEF float impl__scalbnf (float x, int n)
{
    return (float)impl__scalbln((double)x, n);
}

/* -------------------------------------------------------------------------- */

#undef two54
#undef twom54
#undef huge
#undef tiny

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* S_SCALBN_C__ ///////////////////////////////////////////////////////*/

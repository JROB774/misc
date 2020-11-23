/*******************************************************************************
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 *
*******************************************************************************/

#ifndef K_TAN_C__ /*//////////////////////////////////////////////////////////*/
#define K_TAN_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define one     1.00000000000000000000e+00 /* 0x3FF00000, 0x00000000 */
#define pio4    7.85398163397448278999e-01 /* 0x3FE921FB, 0x54442D18 */
#define pio4lo  3.06161699786838301793e-17 /* 0x3C81A626, 0x33145C07 */
#define T00     3.33333333333334091986e-01 /* 0x3FD55555, 0x55555563 */
#define T01     1.33333333333201242699e-01 /* 0x3FC11111, 0x1110FE7A */
#define T02     5.39682539762260521377e-02 /* 0x3FABA1BA, 0x1BB341FE */
#define T03     2.18694882948595424599e-02 /* 0x3F9664F4, 0x8406D637 */
#define T04     8.86323982359930005737e-03 /* 0x3F8226E3, 0xE96E8493 */
#define T05     3.59207910759131235356e-03 /* 0x3F6D6D22, 0xC9560328 */
#define T06     1.45620945432529025516e-03 /* 0x3F57DBC8, 0xFEE08315 */
#define T07     5.88041240820264096874e-04 /* 0x3F4344D8, 0xF2F26501 */
#define T08     2.46463134818469906812e-04 /* 0x3F3026F7, 0x1A8D1068 */
#define T09     7.81794442939557092300e-05 /* 0x3F147E88, 0xA03792A6 */
#define T10     7.14072491382608190305e-05 /* 0x3F12B80F, 0x32F0A7E9 */
#define T11    -1.85586374855275456654e-05 /* 0xBEF375CB, 0xDB605373 */
#define T12     2.59073051863633712884e-05 /* 0x3EFB2A70, 0x74BF7AD4 */

/* -------------------------------------------------------------------------- */

STDDEF double __kernel_tan (double x, double y, int iy)
{
    double z,r,v,w,s;
    int32_t ix,hx;
    GET_HIGH_WORD(hx,x);
    ix = hx&0x7fffffff;
    if (ix<0x3e300000)
    {
        if ((int)x==0)
        {
            u_int32_t low;
          GET_LOW_WORD(low,x);
          if (((ix|low)|(iy+1))==0) return one/impl__fabs(x);
          else                      return (iy==1)? x: -one/x;
        }
    }
    if (ix>=0x3FE59428)
    {
        if (hx<0)
        {
            x = -x;
            y = -y;
        }
        z = pio4-x;
        w = pio4lo-y;
        x = z+w; y = 0.0;
    }
    z = x*x;
    w = z*z;
    r = T01+w*(T03+w*(T05+w*(T07+w*(T09+w*T11))));
    v = z*(T02+w*(T04+w*(T06+w*(T08+w*(T10+w*T12)))));
    s = z*x;
    r = y + z*(s*(r+v)+y);
    r += T00*s;
    w = x+r;
    if (ix>=0x3FE59428)
    {
        v = (double)iy;
        return (double)(1-((hx>>30)&2))*(v-2.0*(x-(w*w/(w+v)-r)));
    }
    if (iy==1) return w;
    else
    {
        double a,t;
        z  = w;
        SET_LOW_WORD(z,0);
        v  = r-(z - x);
        t = a  = -1.0/w;
        SET_LOW_WORD(t,0);
        s  = 1.0+t*z;
        return t+a*(s+t*v);
    }
}

/* -------------------------------------------------------------------------- */

#undef one
#undef pio4
#undef pio4lo
#undef T00
#undef T01
#undef T02
#undef T03
#undef T04
#undef T05
#undef T06
#undef T07
#undef T08
#undef T09
#undef T10
#undef T11
#undef T12

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* K_TAN_C__ //////////////////////////////////////////////////////////*/

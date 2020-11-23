/*******************************************************************************
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 *
*******************************************************************************/

#ifndef S_FLOOR_C__ /*////////////////////////////////////////////////////////*/
#define S_FLOOR_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define huge 1.0e300

/* -------------------------------------------------------------------------- */

STDDEF double impl__floor (double x)
{
	int32_t i0,i1,_j0;
	u_int32_t i,j;
	EXTRACT_WORDS(i0,i1,x);
	_j0 = ((i0>>20)&0x7ff)-0x3ff;
	if (_j0<20)
	{
	    if (_j0<0)
	    {
			if (huge+x>0.0)
			{
			    if (i0>=0) i0=i1=0;
			    else if (((i0&0x7fffffff)|i1)!=0)
				{
					i0=0xbff00000;
					i1=0;
				}
			}
	    }
	    else
	    {
			i = (0x000fffff)>>_j0;
			if (((i0&i)|i1)==0) return x;
			if (huge+x>0.0)
			{
			    if (i0<0) i0 += (0x00100000)>>_j0;
			    i0 &= (~i); i1=0;
			}
	    }
	}
	else if (_j0>51)
	{
	    if (_j0==0x400) return x+x;
	    else            return x;
	}
	else
	{
	    i = ((u_int32_t)(0xffffffff))>>(_j0-20);
	    if ((i1&i)==0) return x;
	    if (huge+x>0.0)
	    {
			if (i0<0)
			{
			    if (_j0==20) i0+=1;
			    else
			    {
					j = i1+(1<<(52-_j0));
					if (j<(u_int32_t)i1) i0 +=1;
					i1=j;
			    }
			}
			i1 &= (~i);
	    }
	}
	INSERT_WORDS(x,i0,i1);
	return x;
}

FILDEF float impl__floorf (float x)
{
	return (float)impl__floor((double)x);
}

/* -------------------------------------------------------------------------- */

#undef huge

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* S_FLOOR_C__ ////////////////////////////////////////////////////////*/

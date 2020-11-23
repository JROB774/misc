/*******************************************************************************
 * Original assembly code taken from SDL2-2.0.10 and modified.
 * Copyright (C) 1997-2019 Sam Lantinga <slouken@libsdl.org>
 *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Shifts a 64-bit unsigned integer to the right.                             //
//                                                                            //
// Parameters:                                                                //
//   [a] - Initial number.                                                    //
//   [b] - Number to shift [a] by to the right.                               //
//                                                                            //
// Returns:                                                                   //
//   The right-shifted value.                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef AULLSHR_C__ /*////////////////////////////////////////////////////////*/
#define AULLSHR_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define impl__aullshr                                \
                                                     \
__asm        cmp         cl,40h                      \
__asm        jae         RETZERO                     \
__asm        cmp         cl,20h                      \
__asm        jae         MORE32                      \
__asm        shrd        eax,edx,cl                  \
__asm        shr         edx,cl                      \
__asm        ret                                     \
__asm    MORE32:                                     \
__asm        mov         eax,edx                     \
__asm        xor         edx,edx                     \
__asm        and         cl,1Fh                      \
__asm        shr         eax,cl                      \
__asm        ret                                     \
__asm    RETZERO:                                    \
__asm        xor         eax,eax                     \
__asm        xor         edx,edx                     \
__asm        ret

/* -------------------------------------------------------------------------- */

void NAKED _aullshr ()
{
    impl__aullshr
}

/* -------------------------------------------------------------------------- */

#endif /* AULLSHR_C__ ////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////////////////////////////////*/

/*******************************************************************************
 *
 * Simple DirectMedia Layer
 * Copyright (C) 1997-2019 Sam Lantinga <slouken@libsdl.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
*******************************************************************************/

/*******************************************************************************
 * Original assembly code taken from SDL2-2.0.10 and modified.
 * Copyright (C) 1997-2019 Sam Lantinga <slouken@libsdl.org>
 *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Multiplies two 64-bit signed integers.                                     //
//                                                                            //
// Parameters:                                                                //
//   [a] - Initial number.                                                    //
//   [b] - Number to multiply [a] by.                                         //
//                                                                            //
// Returns:                                                                   //
//   The product of [a] and [b].                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef ALLMUL_C__ /*/////////////////////////////////////////////////////////*/
#define ALLMUL_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define impl__allmul                                 \
                                                     \
__asm        mov         eax, dword ptr[esp+8]       \
__asm        mov         ecx, dword ptr[esp+10h]     \
__asm        or          ecx, eax                    \
__asm        mov         ecx, dword ptr[esp+0Ch]     \
__asm        jne         hard                        \
__asm        mov         eax, dword ptr[esp+4]       \
__asm        mul         ecx                         \
__asm        ret         10h                         \
__asm    hard:                                       \
__asm        push        ebx                         \
__asm        mul         ecx                         \
__asm        mov         ebx, eax                    \
__asm        mov         eax, dword ptr[esp+8]       \
__asm        mul         dword ptr[esp+14h]          \
__asm        add         ebx, eax                    \
__asm        mov         eax, dword ptr[esp+8]       \
__asm        mul         ecx                         \
__asm        add         edx, ebx                    \
__asm        pop         ebx                         \
__asm        ret         10h

/* -------------------------------------------------------------------------- */

void NAKED _allmul ()
{
    impl__allmul
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* ALLMUL_C__ /////////////////////////////////////////////////////////*/

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

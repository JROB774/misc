/*******************************************************************************
 * Original assembly code taken from SDL2-2.0.10 and modified.
 * Copyright (C) 1997-2019 Sam Lantinga <slouken@libsdl.org>
 *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Divides two 64-bit unsigned integers.                                      //
//                                                                            //
// Parameters:                                                                //
//   [a] - Initial number.                                                    //
//   [b] - Number to divide [a] by.                                           //
//                                                                            //
// Returns:                                                                   //
//   The dividend of [a] and [b].                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef AULLDIV_C__ /*////////////////////////////////////////////////////////*/
#define AULLDIV_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define impl__aulldiv                                \
                                                     \
__asm        push        ebx                         \
__asm        push        esi                         \
__asm        mov         eax,dword ptr [esp+18h]     \
__asm        or          eax,eax                     \
__asm        jne         L1                          \
__asm        mov         ecx,dword ptr [esp+14h]     \
__asm        mov         eax,dword ptr [esp+10h]     \
__asm        xor         edx,edx                     \
__asm        div         ecx                         \
__asm        mov         ebx,eax                     \
__asm        mov         eax,dword ptr [esp+0Ch]     \
__asm        div         ecx                         \
__asm        mov         edx,ebx                     \
__asm        jmp         L2                          \
__asm    L1:                                         \
__asm        mov         ecx,eax                     \
__asm        mov         ebx,dword ptr [esp+14h]     \
__asm        mov         edx,dword ptr [esp+10h]     \
__asm        mov         eax,dword ptr [esp+0Ch]     \
__asm    L3:                                         \
__asm        shr         ecx,1                       \
__asm        rcr         ebx,1                       \
__asm        shr         edx,1                       \
__asm        rcr         eax,1                       \
__asm        or          ecx,ecx                     \
__asm        jne         L3                          \
__asm        div         ebx                         \
__asm        mov         esi,eax                     \
__asm        mul         dword ptr [esp+18h]         \
__asm        mov         ecx,eax                     \
__asm        mov         eax,dword ptr [esp+14h]     \
__asm        mul         esi                         \
__asm        add         edx,ecx                     \
__asm        jb          L4                          \
__asm        cmp         edx,dword ptr [esp+10h]     \
__asm        ja          L4                          \
__asm        jb          L5                          \
__asm        cmp         eax,dword ptr [esp+0Ch]     \
__asm        jbe         L5                          \
__asm    L4:                                         \
__asm        dec         esi                         \
__asm    L5:                                         \
__asm        xor         edx,edx                     \
__asm        mov         eax,esi                     \
__asm    L2:                                         \
__asm        pop         esi                         \
__asm        pop         ebx                         \
__asm        ret         10h

/* -------------------------------------------------------------------------- */

void NAKED _aulldiv ()
{
    impl__aulldiv
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* AULLDIV_C__ ////////////////////////////////////////////////////////*/

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

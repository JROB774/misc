/*******************************************************************************
 * Original assembly code taken from SDL2-2.0.10 and modified.
 * Copyright (C) 1997-2019 Sam Lantinga <slouken@libsdl.org>
 *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Divides two 64-bit signed integers.                                        //
//                                                                            //
// Parameters:                                                                //
//   [a] - Initial number.                                                    //
//   [b] - Number to divide [a] by.                                           //
//                                                                            //
// Returns:                                                                   //
//   The dividend of [a] and [b].                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef ALLDIV_C__ /*/////////////////////////////////////////////////////////*/
#define ALLDIV_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define impl__alldiv                                 \
                                                     \
__asm        push        edi                         \
__asm        push        esi                         \
__asm        push        ebx                         \
__asm        xor         edi,edi                     \
__asm        mov         eax,dword ptr [esp+14h]     \
__asm        or          eax,eax                     \
__asm        jge         L1                          \
__asm        inc         edi                         \
__asm        mov         edx,dword ptr [esp+10h]     \
__asm        neg         eax                         \
__asm        neg         edx                         \
__asm        sbb         eax,0                       \
__asm        mov         dword ptr [esp+14h],eax     \
__asm        mov         dword ptr [esp+10h],edx     \
__asm    L1:                                         \
__asm        mov         eax,dword ptr [esp+1Ch]     \
__asm        or          eax,eax                     \
__asm        jge         L2                          \
__asm        inc         edi                         \
__asm        mov         edx,dword ptr [esp+18h]     \
__asm        neg         eax                         \
__asm        neg         edx                         \
__asm        sbb         eax,0                       \
__asm        mov         dword ptr [esp+1Ch],eax     \
__asm        mov         dword ptr [esp+18h],edx     \
__asm    L2:                                         \
__asm        or          eax,eax                     \
__asm        jne         L3                          \
__asm        mov         ecx,dword ptr [esp+18h]     \
__asm        mov         eax,dword ptr [esp+14h]     \
__asm        xor         edx,edx                     \
__asm        div         ecx                         \
__asm        mov         ebx,eax                     \
__asm        mov         eax,dword ptr [esp+10h]     \
__asm        div         ecx                         \
__asm        mov         edx,ebx                     \
__asm        jmp         L4                          \
__asm    L3:                                         \
__asm        mov         ebx,eax                     \
__asm        mov         ecx,dword ptr [esp+18h]     \
__asm        mov         edx,dword ptr [esp+14h]     \
__asm        mov         eax,dword ptr [esp+10h]     \
__asm    L5:                                         \
__asm        shr         ebx,1                       \
__asm        rcr         ecx,1                       \
__asm        shr         edx,1                       \
__asm        rcr         eax,1                       \
__asm        or          ebx,ebx                     \
__asm        jne         L5                          \
__asm        div         ecx                         \
__asm        mov         esi,eax                     \
__asm        mul         dword ptr [esp+1Ch]         \
__asm        mov         ecx,eax                     \
__asm        mov         eax,dword ptr [esp+18h]     \
__asm        mul         esi                         \
__asm        add         edx,ecx                     \
__asm        jb          L6                          \
__asm        cmp         edx,dword ptr [esp+14h]     \
__asm        ja          L6                          \
__asm        jb          L7                          \
__asm        cmp         eax,dword ptr [esp+10h]     \
__asm        jbe         L7                          \
__asm    L6:                                         \
__asm        dec         esi                         \
__asm    L7:                                         \
__asm        xor         edx,edx                     \
__asm        mov         eax,esi                     \
__asm    L4:                                         \
__asm        dec         edi                         \
__asm        jne         L8                          \
__asm        neg         edx                         \
__asm        neg         eax                         \
__asm        sbb         edx,0                       \
__asm    L8:                                         \
__asm        pop         ebx                         \
__asm        pop         esi                         \
__asm        pop         edi                         \
__asm        ret         10h

/* -------------------------------------------------------------------------- */

void NAKED _alldiv ()
{
    impl__alldiv
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* ALLDIV_C__ /////////////////////////////////////////////////////////*/

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

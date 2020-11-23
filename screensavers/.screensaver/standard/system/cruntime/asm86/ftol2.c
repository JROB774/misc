/*******************************************************************************
 * Original assembly code taken from SDL2-2.0.10 and modified.
 * Copyright (C) 1997-2019 Sam Lantinga <slouken@libsdl.org>
 *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Converts a floating-point value to an integer.                             //
//                                                                            //
// Parameters:                                                                //
//   [a] - Initial number.                                                    //
//                                                                            //
// Returns:                                                                   //
//   An integer representation of [a].                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef FTOL2_C__ /*//////////////////////////////////////////////////////////*/
#define FTOL2_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define impl__ftol2                                  \
                                                     \
__asm        push        ebp                         \
__asm        mov         ebp,esp                     \
__asm        sub         esp,20h                     \
__asm        and         esp,0FFFFFFF0h              \
__asm        fld         st(0)                       \
__asm        fst         dword ptr [esp+18h]         \
__asm        fistp       qword ptr [esp+10h]         \
__asm        fild        qword ptr [esp+10h]         \
__asm        mov         edx,dword ptr [esp+18h]     \
__asm        mov         eax,dword ptr [esp+10h]     \
__asm        test        eax,eax                     \
__asm        je          integer_QnaN_or_zero        \
__asm    arg_is_not_integer_QnaN:                    \
__asm        fsubp       st(1),st                    \
__asm        test        edx,edx                     \
__asm        jns         positive                    \
__asm        fstp        dword ptr [esp]             \
__asm        mov         ecx,dword ptr [esp]         \
__asm        xor         ecx,80000000h               \
__asm        add         ecx,7FFFFFFFh               \
__asm        adc         eax,0                       \
__asm        mov         edx,dword ptr [esp+14h]     \
__asm        adc         edx,0                       \
__asm        jmp         localexit                   \
__asm    positive:                                   \
__asm        fstp        dword ptr [esp]             \
__asm        mov         ecx,dword ptr [esp]         \
__asm        add         ecx,7FFFFFFFh               \
__asm        sbb         eax,0                       \
__asm        mov         edx,dword ptr [esp+14h]     \
__asm        sbb         edx,0                       \
__asm        jmp         localexit                   \
__asm    integer_QnaN_or_zero:                       \
__asm        mov         edx,dword ptr [esp+14h]     \
__asm        test        edx,7FFFFFFFh               \
__asm        jne         arg_is_not_integer_QnaN     \
__asm        fstp        dword ptr [esp+18h]         \
__asm        fstp        dword ptr [esp+18h]         \
__asm    localexit:                                  \
__asm        leave                                   \
__asm        ret

/* -------------------------------------------------------------------------- */

void NAKED _ftol2 ()
{
    impl__ftol2
}

void NAKED _ftol2_sse ()
{
    impl__ftol2
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* FTOL2_C__ //////////////////////////////////////////////////////////*/

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

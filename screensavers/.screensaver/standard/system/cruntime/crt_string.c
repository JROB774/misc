/*******************************************************************************
 * Implementation of portions of the CRT file <string.h> that we desire.
 * Authored By Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//
// THE FUNCTIONS IMPLEMENTED BY THIS FILE ARE AS FOLLOWS:
//
// void* memcpy (void* dst, const void* src, size_t num)
// void* memset (void* dst, int         val, size_t num)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CRT_STRING_C__ /*/////////////////////////////////////////////////////*/
#define CRT_STRING_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#ifdef USE_CRT
#include <string.h>
#endif

/* -------------------------------------------------------------------------- */

#ifndef USE_CRT /**************************************************************/

/* -------------------------------------------------------------------------- */

EXTERN_C_START /***************************************************************/

/* -------------------------------------------------------------------------- */

void* CRTCALL memcpy (void* dst, const void* src, size_t num);
void* CRTCALL memset (void* dst, int         val, size_t num);

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)

#pragma function(memcpy)
void* CRTCALL memcpy (void* dst, const void* src, size_t num)
{
          char* d = (      char*)dst;
    const char* s = (const char*)src;
    while (num--)
    {
        *d++ = *s++;
    }
    return d;
}

#pragma function(memset)
void* CRTCALL memset (void* dst, int val, size_t num)
{
    char* bytes = (char*)dst;
    while (num--)
    {
        *bytes++ = (char)val;
    }
    return dst;
}

/* -------------------------------------------------------------------------- */

EXTERN_C_END /*****************************************************************/

/* -------------------------------------------------------------------------- */

#endif /* !USE_CRT ************************************************************/

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* CRT_STRING_C__ /////////////////////////////////////////////////////*/

/*******************************************************************************
 *
 * Copyright (c) 2020 Joshua Robertson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
*******************************************************************************/

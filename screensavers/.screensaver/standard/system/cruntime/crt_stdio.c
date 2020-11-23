/*******************************************************************************
 * Implementation of portions of the CRT file <stdio.h> that we desire.
 * Authored By Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//
// THE FUNCTIONS IMPLEMENTED BY THIS FILE ARE AS FOLLOWS:
//
// int vsnprintf (char* buf, size_t size, const char* fmt, va_list arg)
// int  vsprintf (char* buf,              const char* fmt, va_list arg)
// int   vprintf (                        const char* fmt, va_list arg)
// int  snprintf (char* buf, size_t size, const char* fmt, ...)
// int   sprintf (char* buf,              const char* fmt, ...)
// int    printf (                        const char* fmt, ...)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CRT_STDIO_C__ /*//////////////////////////////////////////////////////*/
#define CRT_STDIO_C__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#ifdef USE_CRT
#include <stdio.h>
#endif

/* -------------------------------------------------------------------------- */

#ifndef USE_CRT /**************************************************************/

/* -------------------------------------------------------------------------- */

EXTERN_C_START /***************************************************************/

/* -------------------------------------------------------------------------- */

// Makes it so that the functions aren't prefixed with stbsp_
#define STB_SPRINTF_DECORATE(name) name

#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_STATIC

#include <system/cruntime/stb/stb_sprintf.c>

/* -------------------------------------------------------------------------- */

STDDEF int CRTCALL vprintf (const char* fmt, va_list arg)
{
    // Fixed size buffer that can be redefined to avoid need for heap allocations.
    char buf[DEFAULT_CONSOLE_BUFFER_SIZE];

    DWORD  len;
    char*  c;
    DWORD  written;
    HANDLE handle;

    vsnprintf(buf, DEFAULT_CONSOLE_BUFFER_SIZE, fmt, arg);

    // Calculate the length of the string in the buffer.
    len = 0;
    c = buf;
    while (*(c++)) ++len;

    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsole(handle, buf, len, &written, NULL);

    return CAST(int, written);
}

INLDEF int CRTCALL printf (const char* fmt, ...)
{
    va_list arg;
    int written;

    va_start(arg, fmt);
    written = vprintf(fmt, arg);
    va_end(arg);

    return written;
}

/* -------------------------------------------------------------------------- */

EXTERN_C_END /*****************************************************************/

/* -------------------------------------------------------------------------- */

#endif /* !USE_CRT ************************************************************/

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* CRT_STDIO_C__ //////////////////////////////////////////////////////*/

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

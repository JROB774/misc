/*******************************************************************************
 * Implementation of custom debugging assertion and static assertion macros.
 * Authored by Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

#ifndef ASSERT_HPP__ /*///////////////////////////////////////////////////////*/
#define ASSERT_HPP__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#if defined(COMPILER_HAS_STDCPP17)
#define STATIC_ASSERT(e, ...) static_assert(e, __VA_ARGS__)
#else
#define STATIC_ASSERT(e, ...) typedef char STATIC_ASSERT__[(e)?1:-1]
#endif

/* -------------------------------------------------------------------------- */

#if defined(BUILD_DEBUG)
#define ASSERT(e) assert__(e,                                     \
    "Assertion in " __FILE__ " at line " STRINGIFY(__LINE__) "\n" \
    "Condition: " STRINGIFY(e))
#else
#define ASSERT(e) ((void)0)
#endif

/* -------------------------------------------------------------------------- */

#if defined(PLATFORM_WIN32) /**************************************************/

#include <windows.h>

STDDEF void assert__ (bool condition, const char* str)
{
    if (!condition)
    {
        if (!IsDebuggerPresent())
        {
            if (MessageBoxA(NULL, str, "Debug Assertion",
                MB_RETRYCANCEL|MB_ICONWARNING) == IDCANCEL)
            {
                ExitProcess(1);
            }
        }
        else
        {
            OutputDebugStringA(str);
            DebugBreak();
        }
    }
}

#endif /* PLATFORM_WIN32 ******************************************************/

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* ASSERT_HPP__ ///////////////////////////////////////////////////////*/

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

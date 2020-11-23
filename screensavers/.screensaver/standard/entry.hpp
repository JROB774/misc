/*******************************************************************************
 * Includes the platform-specific custom entry-point for the application.
 * Authored By Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

#ifndef MAIN_HPP__ /*/////////////////////////////////////////////////////////*/
#define MAIN_HPP__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#include <system/compiler.h>
#include <system/platform.h>
#include <system/standard.h>
#include <system/cruntime.h>

/* -------------------------------------------------------------------------- */

// Prototype the user's main functions so that we can call it from here.
STDDEF int internal__entry_point ();

/* -------------------------------------------------------------------------- */

#if defined(USE_SDL)
#include <system/entry/sdl.cpp>
#else
#if defined(PLATFORM_WIN32)
#include <system/entry/win32.cpp>
#endif
#endif // USE_SDL

/* -------------------------------------------------------------------------- */

#define main internal__entry_point

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* MAIN_HPP__ /////////////////////////////////////////////////////////*/

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

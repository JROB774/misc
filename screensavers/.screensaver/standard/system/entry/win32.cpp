/*******************************************************************************
 * Program entry-point and system setup for the Win32 platform.
 * Authored By Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#include <windows.h>

/* -------------------------------------------------------------------------- */

#if !defined(USE_CRT) /********************************************************/

void WinMainCRTStartup ()
{
    ExitProcess(internal__entry_point());
}

void mainCRTStartup ()
{
    ExitProcess(internal__entry_point());
}

#endif /* !USE_CRT ************************************************************/

/* -------------------------------------------------------------------------- */

#if defined(USE_CRT) /*********************************************************/

int WINAPI WinMain (HINSTANCE instance, HINSTANCE prev, LPSTR cmdline, int show)
{
    return internal__entry_point();
}

int main (int argc, char** argv)
{
    return internal__entry_point();
}

int wmain (int argc, wchar_t** wargv, wchar_t* wenvp)
{
    return internal__entry_point();
}

#endif /* USE_CRT *************************************************************/

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

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

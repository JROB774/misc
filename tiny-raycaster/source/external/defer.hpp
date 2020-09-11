/*******************************************************************************
 * Single-file implementation of defer functionality as a macro.
 * Available Under Public Domain or MIT License (See EOF)
 * Released 24-08-2020
 * Version 1.0.3
 * Authored by Joshua Robertson
 *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//
// Allows for code to be deferred to the end of scope for later execution.
// An example of the syntax for using the custom defer macro utility is:
//
// void my_function ()
// {
//     FILE* file = fopen("myfile.txt", "r");
//     defer { fclose(file); };
//     // Do stuff with file...
//     // File closed on exit...
// }
//
// This allows for a clear and easy method of executing tasks during scope
// exit -- often including freeing memory, other resources, and whatever.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DEFER_HPP__ /*////////////////////////////////////////////////////////*/
#define DEFER_HPP__

/* -------------------------------------------------------------------------- */

#if !defined(__cplusplus) || (__cplusplus < 201103L)
#error C++11 is required in order to implement the defer macro!
#endif

/* -------------------------------------------------------------------------- */

#define DEFER_VERSION_MAJOR 1
#define DEFER_VERSION_MINOR 0
#define DEFER_VERSION_PATCH 3

#ifdef __COUNTER__
#define defer \
const auto& DEFER_CONCAT2(defer, __COUNTER__) = defer_::Defer_Help() + [&]()
#else
#define defer \
const auto& DEFER_CONCAT2(defer,    __LINE__) = defer_::Defer_Help() + [&]()
#endif

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////// IMPLEMENTATION //////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define DEFER_CONCAT2(a, b) DEFER_CONCAT1(a, b)
#define DEFER_CONCAT1(a, b) a##b

namespace defer_
{

template<typename T>
struct Defer
{
    T lambda;

             Defer   (T block): lambda(block) { /* Nothing! */ }
            ~Defer   ()                       {    lambda();   }

             Defer   (const Defer& d) = delete;
    Defer& operator= (const Defer& d) = delete;
};
struct Defer_Help
{
    template<typename T>
    Defer<T> operator+ (T type) { return type; }
};

} // defer_

/* -------------------------------------------------------------------------- */

#endif /* DEFER_HPP__ ////////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////*/

/*******************************************************************************
 *
 * REVISION HISTORY:
 *
 * v1.0.3  [24-08-2020]  Fixed a compile-time naming bug.
 * v1.0.2  [31-03-2020]  Changed some of the source code formatting.
 * v1.0.1  [26-11-2019]  Added support for __COUNTER__ define.
 * v1.0.0  [24-11-2019]  Initial release.
 *
*******************************************************************************/

/*******************************************************************************
 *
 * THIS SOFTWARE IS AVAILABLE UNDER 2 LICENSES -- CHOOSE WHICHEVER YOU PREFER.
 *
 *******************************************************************************
 *
 * ALTERNATIVE A - MIT License
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
 *******************************************************************************
 *
 * ALTERNATIVE B - Public Domain
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
*******************************************************************************/

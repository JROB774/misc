/*******************************************************************************
 * Allows for sections of code to be deferred to the end of the current scope.
 * Authored by Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Allows for code to be deferred to the end of scope for later execution.    //
// An example of the syntax for using the custom defer macro utility is:      //
//                                                                            //
// void my_function ()                                                        //
// {                                                                          //
//     FILE* file = fopen("myfile.txt", "r");                                 //
//     defer { fclose(file); };                                               //
//     // Do stuff with file...                                               //
//     // File closed on exit...                                              //
// }                                                                          //
//                                                                            //
// This allows for a clear and easy method of executing tasks during scope    //
// exit -- often including freeing memory, other resources, and whatever.     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef DEFER_HPP__ /*////////////////////////////////////////////////////////*/
#define DEFER_HPP__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#ifndef COMPILER_HAS_STDCPP11
#error C++11 support is required in order to implement the defer macro!
#endif

/* -------------------------------------------------------------------------- */

#ifdef __COUNTER__
#define defer \
const auto& JOIN(defer, __COUNTER__) = Defer_Help__() + [&]()
#else
#define defer \
const auto& JOIN(defer,    __LINE__) = Defer_Help__() + [&]()
#endif

template<typename T>
struct Defer__
{
    T lambda;

               Defer__ (T lambda): lambda(lambda) { /* Nothing! */ }
              ~Defer__ ()                         {    lambda();   }

               Defer__ (const Defer__& object) = delete;
    Defer__& operator= (const Defer__& object) = delete;
};
struct Defer_Help__
{
    template<typename T>
    Defer__<T> operator+ (T type) { return type; }
};

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* DEFER_HPP__ ////////////////////////////////////////////////////////*/

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

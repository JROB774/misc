/*******************************************************************************
 * General-purpose utility functions, defines, macros and types.
 * Authored by Joshua Robertson
 * Available Under MIT License (See EOF)
 *
*******************************************************************************/

#ifndef BASIC_HPP__ /*////////////////////////////////////////////////////////*/
#define BASIC_HPP__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

#include <assert.hpp>

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define STATIC_ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

/* -------------------------------------------------------------------------- */

// @Cleanup: Move these out into a special time.hpp file at some point.

#if defined(PLATFORM_WIN32)
FILDEF u64 get_performance_frequency ()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency.QuadPart;
}
#else
#error get_performance_frequency() only implemented on PLATFORM_WIN32
#endif

#if defined(PLATFORM_WIN32)
FILDEF u64 get_performance_counter ()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return counter.QuadPart;
}
#else
#error get_performance_counter() only implemented on PLATFORM_WIN32
#endif

/* -------------------------------------------------------------------------- */

// @Cleanup: Move these out into a special memory.hpp file at some point.

// @Temporary: Just the most simple possible memory allocation for now. Will
//             want to implement proper memory allocator(s) in the future.

#define allocate(type, size) CAST(type*, allocate_impl((size)*sizeof(type)))

#if defined(PLATFORM_WIN32)
FILDEF void* allocate_impl (size_t size)
{
    return VirtualAlloc(NULL, size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
}
#else
#error allocate_impl() only implemented on PLATFORM_WIN32
#endif

#define deallocate(ptr) deallocate_impl(ptr)

#if defined(PLATFORM_WIN32)
FILDEF void deallocate_impl (void* ptr)
{
    VirtualFree(ptr, 0, MEM_RESERVE);
}
#else
#error deallocate_impl() only implemented on PLATFORM_WIN32
#endif

/* -------------------------------------------------------------------------- */

struct File_Content
{
    size_t size;
    void* data;
};

#if defined(PLATFORM_WIN32)
STDDEF File_Content read_entire_file_content (const char* file_name)
{
    File_Content file_content = {};

    HANDLE file = CreateFileA(file_name, GENERIC_READ, NULL,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file != INVALID_HANDLE_VALUE)
    {
        file_content.size = GetFileSize(file, NULL);
        if (file_content.size)
        {
            file_content.data = allocate(u8, file_content.size);
            if (file_content.data)
            {
                DWORD bytes_read = 0;
                ReadFile(file, file_content.data, CAST(DWORD,
                    file_content.size), &bytes_read, NULL);
                if (bytes_read != file_content.size)
                {
                    ASSERT(bytes_read != file_content.size); // @Error
                }
            }
        }

        CloseHandle(file);
    }

    return file_content;
}
#else
#error read_entire_file_content() only implemented on PLATFORM_WIN32
#endif

STDDEF void free_file_content (File_Content* file_content)
{
    deallocate(file_content->data);

    file_content->data = NULL;
    file_content->size = 0;
}

/* -------------------------------------------------------------------------- */

// @Cleanup: Move these out into a special random.hpp file at some point.

// @Temporary: This stuff is probably temporary or we'll keep it as a simplistic
//             random number generator and just change the names. We will want
//             to implement some better pseudo-random facilities at some point.

GLOBAL unsigned int g_current_random_value = 1;

FILDEF void set_random_seed (unsigned int seed)
{
    g_current_random_value = seed;
}

FILDEF int random_int ()
{
    g_current_random_value = g_current_random_value * 1103515245 + 12345;
    return CAST(int, (g_current_random_value / 65536) % 32768);
}

/* -------------------------------------------------------------------------- */

STDDEF bool cstr_equals (const char* a, const char* b)
{
    while (*a && *b) if (*a++ != *b++) return false;
    return (*b == NULL);
}

STDDEF size_t cstr_length (const char* str)
{
    size_t len = 0;
    if (str) while (*(str++)) ++len;
    return len;
}

STDDEF const char* cstr_find (const char* str, const char* to_find)
{
    while (*str)
    {
        if ((*str && *to_find) && cstr_equals(str, to_find)) return str;
        str++;
    }
    return NULL;
}
STDDEF char* cstr_find (char* str, const char* to_find)
{
    while (*str)
    {
        if ((*str && *to_find) && cstr_equals(str, to_find)) return str;
        str++;
    }
    return NULL;
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* BASIC_HPP__ ////////////////////////////////////////////////////////*/

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

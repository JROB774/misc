/*******************************************************************************
 * Single-file implementation of a TGA image file format loader.
 * Available Under Public Domain or MIT License (See EOF)
 * Released 11-11-2020
 * Version 1.0.1
 * Authored by Joshua Robertson
 *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//
// Useful resources for implementing the TGA file format specification:
//   https://en.wikipedia.org/wiki/Truevision_TGA
//   http://paulbourke.net/dataformats/tga/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TGA_LOADER_H__ /*/////////////////////////////////////////////////////*/
#define TGA_LOADER_H__

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#define TGA_LOADER_VERSION_MAJOR 1
#define TGA_LOADER_VERSION_MINOR 0
#define TGA_LOADER_VERSION_PATCH 1

#include <stddef.h>
#include <stdint.h>

#ifndef TGA_NO_CRT
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#endif

#ifdef TGA_LOADER_STATIC
#define TGADEF static
#else
#define TGADEF extern
#endif

// Redefine these if you want to use custom memory allocators and assertion.
// If you have defined TGA_NO_CRT it is necessary to define these macros.
#ifndef TGA_MEMCPY
#define TGA_MEMCPY(dst,src,sz) memcpy(dst,src,sz)
#endif
#ifndef TGA_MALLOC
#define TGA_MALLOC(sz) malloc(sz)
#endif
#ifndef TGA_FREE
#define TGA_FREE(ptr) free(ptr)
#endif
#ifndef TGA_ASSERT
#define TGA_ASSERT(e) assert(e)
#endif

// If the CRT is not being used then this library expects a macro that
// takes in a file name and a TGA_File_Content* and fills in the size and
// data members of the struct. The returned data inside the file content
// struct should be freeable using the macro TGA_FREE.
// See the function tga_internal_read_entire_file_content for an example
// of how this macro will be used within the internals of this library.
#ifdef TGA_NO_CRT
#ifndef TGA_READ_ENTIRE_FILE
#error TGA_READ_ENTIRE_FILE must be defined!
#endif
#endif

/* -------------------------------------------------------------------------- */

TGADEF uint8_t* tga_load_from_file (const char* file_name,      int* w, int* h, int* bpp);
TGADEF uint8_t* tga_load_from_mem  (const void* buf, size_t sz, int* w, int* h, int* bpp);
TGADEF void     tga_free           (uint8_t* pixels);

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

/* -------------------------------------------------------------------------- */

#endif /* TGA_LOADER_H__ /////////////////////////////////////////////////////*/

/*////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////// IMPLEMENTATION //////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////*/

#ifdef TGA_LOADER_IMPLEMENTATION /*///////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#define TGA_COLOR_MAP_TYPE_NONE           0
#define TGA_COLOR_MAP_TYPE_PRESENT        1

#define TGA_IMAGE_TYPE_NO_DATA            0 // Unsupported.
#define TGA_IMAGE_TYPE_UNCOMP_COLOR_MAP   1 // Unsupported.
#define TGA_IMAGE_TYPE_UNCOMP_TRUE_COLOR  2
#define TGA_IMAGE_TYPE_UNCOMP_GREYSCALE   3 // Unsupported.
#define TGA_IMAGE_TYPE_RLE_COLOR_MAP      9 // Unsupported.
#define TGA_IMAGE_TYPE_RLE_TRUE_COLOR    10
#define TGA_IMAGE_TYPE_RLE_GREYSCALE     11 // Unsupported.

/* -------------------------------------------------------------------------- */

// Not part of the TGA format spec, but used by us to load TGA data.
typedef struct TGA_Stream_Buffer__
{
    size_t size;
    const void* data;
} TGA_Stream_Buffer;
typedef struct TGA_File_Content__
{
    size_t size;
    void* data;
}  TGA_File_Content;

/* -------------------------------------------------------------------------- */

// Structs that map to portions of the TGA file format specification.
#pragma pack(push, 1)
typedef struct TGA_Color_Map_Spec__
{
    uint16_t           first_entry_index;
    uint16_t           length;
     uint8_t           entry_size;
} TGA_Color_Map_Spec;
typedef struct TGA_Image_Spec__
{
    uint16_t           x_origin;
    uint16_t           y_origin;
    uint16_t           width;
    uint16_t           height;
     uint8_t           pixel_depth; // In bits per pixel.
     uint8_t           descriptor;
} TGA_Image_Spec;
typedef struct TGA_Header__
{
    uint8_t            id_length;
    uint8_t            color_map_type;
    uint8_t            image_type;
    TGA_Color_Map_Spec color_map_spec;
    TGA_Image_Spec     image_spec;
} TGA_Header;
#pragma pack(pop)

/* -------------------------------------------------------------------------- */

static const void* tga_internal_stream_consume_bytes (TGA_Stream_Buffer* stream, size_t size)
{
    const void* bytes = NULL;
    if (stream->size >= size)
    {
        bytes = stream->data;
        stream->data  = ((const uint8_t*)(stream->data) + size);
        stream->size -= size;
    }
    else
    {
        // File underflow error caught here!
        TGA_ASSERT(0);
        stream->size = 0;
    }
    return bytes;
}

#define tga_internal_stream_consume_type(stream, type) \
((type*)tga_internal_stream_consume_bytes(stream, sizeof(type)))

static TGA_File_Content tga_internal_read_entire_file_content (const char* file_name)
{
    TGA_File_Content content;
    content.data = NULL;
    content.size = 0;

    #ifdef TGA_NO_CRT
    TGA_READ_ENTIRE_FILE(file_name, &content);
    #else
    FILE* file = fopen(file_name, "rb");
    if (file)
    {
        fseek(file, 0L, SEEK_END);
        content.size = ftell(file);
        rewind(file);

        content.data = TGA_MALLOC(content.size);
        if (content.data)
        {
            fread(content.data, content.size, 1, file);
            fclose(file);
        }
    }
    #endif // TGA_NO_CRT

    return content;
}

static void tga_internal_free_file_content (TGA_File_Content* content)
{
    TGA_FREE(content->data);
    content->data = NULL;
    content->size = 0;
}

/* -------------------------------------------------------------------------- */

TGADEF uint8_t* tga_load_from_file (const char* file_name, int* w, int* h, int* bpp)
{
    TGA_File_Content file = tga_internal_read_entire_file_content(file_name);

    TGA_ASSERT(file.data);
    TGA_ASSERT(file.size);

    uint8_t* pixels = tga_load_from_mem(file.data, file.size, w, h, bpp);

    tga_internal_free_file_content(&file);

    return pixels;
}

TGADEF uint8_t* tga_load_from_mem (const void* buf, size_t sz, int* w, int* h, int* bpp)
{
    TGA_Stream_Buffer stream;

    stream.data = buf;
    stream.size = sz;

    TGA_ASSERT(stream.data);
    TGA_ASSERT(stream.size);

    uint8_t* pixels = NULL;

    TGA_Header* header = tga_internal_stream_consume_type(&stream, TGA_Header);
    if (header)
    {
        // If the TGA has no data then we don't need to continue parsing the file.
        if (header->image_type != TGA_IMAGE_TYPE_NO_DATA)
        {
            // Consume the image identifier even though we have no use for it.
            tga_internal_stream_consume_bytes(&stream, header->id_length);

            int width  = header->image_spec.width;
            int height = header->image_spec.height;
            int depth  = header->image_spec.pixel_depth / 8;
            int pitch  = width * depth;

            pixels = (uint8_t*)TGA_MALLOC(width*height*depth);
            if (pixels)
            {
                switch (header->image_type)
                {
                    case (TGA_IMAGE_TYPE_UNCOMP_COLOR_MAP):
                    {
                        // Unsupported TGA file format type.
                        TGA_ASSERT(0);
                    } break;
                    case (TGA_IMAGE_TYPE_UNCOMP_TRUE_COLOR):
                    {
                        uint8_t* dst = pixels;
                        for (int i=0; i<height; ++i)
                        {
                            uint8_t* src = (uint8_t*)tga_internal_stream_consume_bytes(&stream, pitch);
                            if (src)
                            {
                                TGA_MEMCPY(dst, src, pitch);
                                dst += pitch;
                            }
                        }
                    } break;
                    case (TGA_IMAGE_TYPE_UNCOMP_GREYSCALE):
                    {
                        // Unsupported TGA file format type.
                        TGA_ASSERT(0);
                    } break;
                    case (TGA_IMAGE_TYPE_RLE_COLOR_MAP):
                    {
                        // Unsupported TGA file format type.
                        TGA_ASSERT(0);
                    } break;
                    case (TGA_IMAGE_TYPE_RLE_TRUE_COLOR):
                    {
                        int pixel_count = 0;
                        uint8_t* dst = pixels;
                        while (pixel_count < (width*height))
                        {
                            uint8_t* rle_packet = tga_internal_stream_consume_type(&stream, uint8_t);
                            if (rle_packet)
                            {
                                uint8_t rle_header = (((*rle_packet) & 0x80) >> 7);
                                if (rle_header)
                                {
                                    uint8_t num_repeat = ((*rle_packet) & 0x7F) + 1;
                                    pixel_count += num_repeat;

                                    uint8_t* src = (uint8_t*)tga_internal_stream_consume_bytes(&stream, depth);
                                    for (int i=0; i<num_repeat; ++i)
                                    {
                                        TGA_MEMCPY(dst, src, depth);
                                        dst += (depth);
                                    }
                                }
                                else
                                {
                                    uint8_t num_pixels = ((*rle_packet) & 0x7F) + 1;
                                    pixel_count += num_pixels;

                                    int bytes = num_pixels*depth;
                                    uint8_t* src = (uint8_t*)tga_internal_stream_consume_bytes(&stream, bytes);

                                    TGA_MEMCPY(dst, src, bytes);
                                    dst += (bytes);
                                }
                            }
                        }
                    } break;
                    case (TGA_IMAGE_TYPE_RLE_GREYSCALE):
                    {
                        // Unsupported TGA file format type.
                        TGA_ASSERT(0);
                    } break;
                }

                if (w  ) *w   = width;
                if (h  ) *h   = height;
                if (bpp) *bpp = depth;
            }
        }
    }

    return pixels;
}

TGADEF void tga_free (uint8_t* pixels)
{
    TGA_FREE(pixels);
}

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

/* -------------------------------------------------------------------------- */

#endif /* TGA_LOADER_IMPLEMENTATION //////////////////////////////////////////*/

/*////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////////////////////*/

/*******************************************************************************
 *
 * REVISION HISTORY:
 *
 * v1.0.1  [11-11-2020]  Added define TGA_NO_CRT for disabling inclusion of CRT.
 *                       Added macros for replacing CRT function calls.
 *                       Now defining our structs with the typedef keyword.
 *                       Removed the use of false keyword in TGA_ASSERT calls.
 * v1.0.0  [24-08-2020]  Initial release.
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

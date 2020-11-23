#pragma once

////////////////////////////////////////////////////////////////////////////////
//
// Useful resources for implementing the TGA file format specification:
//   https://en.wikipedia.org/wiki/Truevision_TGA
//   http://paulbourke.net/dataformats/tga/
//
////////////////////////////////////////////////////////////////////////////////

GLOBAL constexpr u8 TGA_COLOR_MAP_TYPE_NONE    = 0;
GLOBAL constexpr u8 TGA_COLOR_MAP_TYPE_PRESENT = 1;

GLOBAL constexpr u8 TGA_IMAGE_TYPE_NO_DATA           =  0; // Unsupported.
GLOBAL constexpr u8 TGA_IMAGE_TYPE_UNCOMP_COLOR_MAP  =  1; // Unsupported.
GLOBAL constexpr u8 TGA_IMAGE_TYPE_UNCOMP_TRUE_COLOR =  2;
GLOBAL constexpr u8 TGA_IMAGE_TYPE_UNCOMP_GREYSCALE  =  3; // Unsupported.
GLOBAL constexpr u8 TGA_IMAGE_TYPE_RLE_COLOR_MAP     =  9; // Unsupported.
GLOBAL constexpr u8 TGA_IMAGE_TYPE_RLE_TRUE_COLOR    = 10;
GLOBAL constexpr u8 TGA_IMAGE_TYPE_RLE_GREYSCALE     = 11; // Unsupported.

#pragma pack(push, 1) //////////////////////////////////////////////////////////

struct TGA_Color_Map_Spec
{
    u16 first_entry_index;
    u16 length;
    u8  entry_size;
};

struct TGA_Image_Spec
{
    u16 x_origin;
    u16 y_origin;
    u16 width;
    u16 height;
    u8  pixel_depth; // In bits per pixel.
    u8  descriptor;
};

struct TGA_Header
{
    u8                 id_length;
    u8                 color_map_type;
    u8                 image_type;
    TGA_Color_Map_Spec color_map_spec;
    TGA_Image_Spec     image_spec;
};

// Not part of the TGA format spec, but used by us to load the TGA data.
struct TGA_Stream_Buffer
{
    size_t size;
    const void* data;
};

#pragma pack(pop) //////////////////////////////////////////////////////////////

INLDEF u8* load_tga_from_file (const char* file_name, int* w, int* h, int* bpp);
STDDEF u8* load_tga_from_mem  (const void* buf, size_t sz, int* w, int* h, int* bpp);

FILDEF void free_tga (u8* pixels);

#ifndef __STB_H_INCLUDED__
#define __STB_H_INCLUDED__

// prevent warnings from stb libraries on gcc/g++
#ifdef __GNUC__
#pragma GCC system_header
#endif

// required before including stb libs
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// local includes
#include "stb_image.h"
#include "stb_image_write.h"

#endif // __STB_H_INCLUDED__
#pragma once

typedef  uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef   int8_t  s8;
typedef  int16_t s16;
typedef  int32_t s32;

static void unpack_color (u32 color, u8& r, u8& g, u8& b, u8& a);
static u32  pack_color   (u8 r, u8 g, u8 b, u8 a = 0xFF);
static void save_ppm     (const std::string file_name, const std::vector<u32>& pixels, int w, int h);

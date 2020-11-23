////////////////////////////////////////////////////////////////////////////////

STDDEF const void* intern__tga_stream_consume_bytes (TGA_Stream_Buffer* stream, size_t size)
{
    const void* bytes = NULL;
    if (stream->size >= size)
    {
        bytes = stream->data;
        stream->data  = (CAST(const u8*, stream->data) + size);
        stream->size -= size;
    }
    else
    {
        // printf("File Underflow!\n"); // @Error
        stream->size = 0;
    }
    return bytes;
}

#define intern__tga_stream_consume_type(stream, type) \
CAST(type*, intern__tga_stream_consume_bytes(stream, sizeof(type)))

////////////////////////////////////////////////////////////////////////////////

INLDEF u8* load_tga_from_file (const char* file_name, int* w, int* h, int* bpp)
{
    File_Content file = read_entire_file_content(file_name);

    ASSERT(file.data);
    ASSERT(file.size);

    u8* pixels = load_tga_from_mem(file.data, file.size, w, h, bpp);

    free_file_content(&file);

    return pixels;
}

STDDEF u8* load_tga_from_mem (const void* buf, size_t sz, int* w, int* h, int* bpp)
{
    TGA_Stream_Buffer stream;

    stream.data = buf;
    stream.size = sz;

    ASSERT(stream.data);
    ASSERT(stream.size);

    u8* pixels = NULL;

    TGA_Header* header = intern__tga_stream_consume_type(&stream, TGA_Header);
    if (header)
    {
        // Consume the image identifier even though we have no use for it.
        intern__tga_stream_consume_bytes(&stream, header->id_length);

        int width  = header->image_spec.width;
        int height = header->image_spec.height;
        int depth  = header->image_spec.pixel_depth / 8;
        int pitch  = width * depth;

        if (header->image_type == TGA_IMAGE_TYPE_UNCOMP_TRUE_COLOR ||
            header->image_type == TGA_IMAGE_TYPE_RLE_TRUE_COLOR)
        {
            pixels = allocate(u8, width*height*depth);
            if (pixels)
            {
                switch (header->image_type)
                {
                    case (TGA_IMAGE_TYPE_UNCOMP_TRUE_COLOR):
                    {
                        u8* dst = pixels;
                        for (int i=0; i<height; ++i)
                        {
                            u8* src = CAST(u8*, intern__tga_stream_consume_bytes(&stream, pitch));
                            if (src)
                            {
                                memcpy(dst, src, pitch);
                                dst += pitch;
                            }
                        }
                    } break;
                    case (TGA_IMAGE_TYPE_RLE_TRUE_COLOR):
                    {
                        int pixel_count = 0;
                        u8* dst = pixels;
                        while (pixel_count < (width*height))
                        {
                            u8* rle_packet = intern__tga_stream_consume_type(&stream, u8);
                            if (rle_packet)
                            {
                                u8 rle_header = (((*rle_packet) & 0x80) >> 7);
                                if (rle_header)
                                {
                                    u8 num_repeat = ((*rle_packet) & 0x7F) + 1;
                                    pixel_count += num_repeat;

                                    u8* src = CAST(u8*, intern__tga_stream_consume_bytes(&stream, depth));
                                    for (int i=0; i<num_repeat; ++i)
                                    {
                                        memcpy(dst, src, depth);
                                        dst += (depth);
                                    }
                                }
                                else
                                {
                                    u8 num_pixels = ((*rle_packet) & 0x7F) + 1;
                                    pixel_count += num_pixels;

                                    int bytes = num_pixels*depth;
                                    u8* src = CAST(u8*, intern__tga_stream_consume_bytes(&stream, bytes));

                                    memcpy(dst, src, bytes);
                                    dst += (bytes);
                                }
                            }
                        }
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

FILDEF void free_tga (u8* pixels)
{
    deallocate(pixels);
}

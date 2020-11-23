#ifndef VERTEX_BUFFER_HPP__ /*////////////////////////////////////////////////*/
#define VERTEX_BUFFER_HPP__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

typedef GLenum Vertex_Buffer_Usage;
typedef GLenum Vertex_Buffer_Mode;

GLOBAL constexpr Vertex_Buffer_Usage BUFFER_STREAM      = GL_STREAM_DRAW;
GLOBAL constexpr Vertex_Buffer_Usage BUFFER_DYNAMIC     = GL_DYNAMIC_DRAW;
GLOBAL constexpr Vertex_Buffer_Usage BUFFER_STATIC      = GL_STATIC_DRAW;

GLOBAL constexpr Vertex_Buffer_Mode DRAW_POINTS         = GL_POINTS;
GLOBAL constexpr Vertex_Buffer_Mode DRAW_LINE_STRIP     = GL_LINE_STRIP;
GLOBAL constexpr Vertex_Buffer_Mode DRAW_LINE_LOOP      = GL_LINE_LOOP;
GLOBAL constexpr Vertex_Buffer_Mode DRAW_TRIANGLE_STRIP = GL_TRIANGLE_STRIP;
GLOBAL constexpr Vertex_Buffer_Mode DRAW_TRIANGLE_FAN   = GL_TRIANGLE_FAN;
GLOBAL constexpr Vertex_Buffer_Mode DRAW_TRIANGLES      = GL_TRIANGLE_STRIP;

typedef GLuint VAO;
typedef GLuint VBO;

struct Vertex_Buffer
{
    Vertex_Buffer_Mode mode;

    VAO vao;
    VBO vbo;
};

STDDEF bool create_vertex_buffer (Vertex_Buffer& buffer);
STDDEF void free_vertex_buffer   (Vertex_Buffer& buffer);

STDDEF void draw_buffer (Vertex_Buffer& buffer, int vert_count);

// NOTE: We assume the vertex data to always be of type GL_FLOAT when setting.
STDDEF void set_buffer_attrib (Vertex_Buffer& buffer, int index, int size,
                               bool normal, size_t stride, size_t offset);
STDDEF void set_buffer_data   (Vertex_Buffer& buffer, const void* data,
                               size_t size, Vertex_Buffer_Usage usage);
/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* VERTEX_BUFFER_HPP__ ////////////////////////////////////////////////*/

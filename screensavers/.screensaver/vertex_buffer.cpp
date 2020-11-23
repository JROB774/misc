/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

STDDEF bool create_vertex_buffer (Vertex_Buffer& buffer)
{
    glGenVertexArrays(1, &buffer.vao);
    glBindVertexArray(buffer.vao);

    glGenBuffers(1, &buffer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);

    // We just use DRAW_TRIANGLE_STRIP as the default drawing mode.
    buffer.mode = DRAW_TRIANGLE_STRIP;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

STDDEF void free_vertex_buffer (Vertex_Buffer& buffer)
{
    glDeleteVertexArrays(1, &buffer.vao);
    glDeleteBuffers(1, &buffer.vbo);
}

STDDEF void draw_buffer (Vertex_Buffer& buffer, int vert_count)
{
    glBindVertexArray(buffer.vao);
    glDrawArrays(buffer.mode, 0, vert_count);
    glBindVertexArray(0);
}

STDDEF void set_buffer_attrib (Vertex_Buffer& buffer, int index, int size, bool normal, size_t stride, size_t offset)
{
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);

    glVertexAttribPointer(index, size, GL_FLOAT, normal, sizeof(float)*CAST(GLsizei,stride), CAST(void*,offset*sizeof(float)));
    glEnableVertexAttribArray(index);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

STDDEF void set_buffer_data (Vertex_Buffer& buffer, const void* data, size_t size, Vertex_Buffer_Usage usage)
{
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);

    glBufferData(GL_ARRAY_BUFFER, size, data, usage);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

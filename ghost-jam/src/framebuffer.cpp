namespace internal
{
    const GLOBAL GLfloat FRAMEBUFFER_VERTS[]
    {
         // Position   // TexCoord
        -1.0f,  1.0f,  0.0f, 1.0f, // BL
        -1.0f, -1.0f,  0.0f, 0.0f, // TL
         1.0f,  1.0f,  1.0f, 1.0f, // BR
         1.0f, -1.0f,  1.0f, 0.0f  // TR
    };

    typedef GLuint RBO;

    struct Framebuffer
    {
        GLuint handle;
        GLuint texture;

        VAO vao;
        VBO vbo;
        RBO rbo;

        float width;
        float height;

        std::string effect;

        bool enabled;
    };

    GLOBAL Framebuffer framebuffer;
}

FILDEF void generate_framebuffer (int _w, int _h)
{
    internal::framebuffer.enabled = false;

    glGenVertexArrays(1, &internal::framebuffer.vao);
    glGenBuffers(1, &internal::framebuffer.vbo);

    glBindVertexArray(internal::framebuffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, internal::framebuffer.vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(internal::FRAMEBUFFER_VERTS), &internal::FRAMEBUFFER_VERTS, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(0*sizeof(GLfloat)));
    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // TexCoord

    resize_framebuffer(_w, _h);
}

FILDEF void destroy_framebuffer ()
{
    disable_framebuffer();

    glDeleteFramebuffers(1, &internal::framebuffer.handle);
    glDeleteTextures(1, &internal::framebuffer.texture);
    glDeleteRenderbuffers(1, &internal::framebuffer.rbo);

    glDeleteVertexArrays(1, &internal::framebuffer.vao);
    glDeleteBuffers(1, &internal::framebuffer.vbo);

    internal::framebuffer.width = 0.0f;
    internal::framebuffer.height = 0.0f;
}

FILDEF void resize_framebuffer (int _w, int _h)
{
    glDeleteFramebuffers(1, &internal::framebuffer.handle);
    glDeleteTextures(1, &internal::framebuffer.texture);
    glDeleteRenderbuffers(1, &internal::framebuffer.rbo);

    //////////////////////////////////////////////////////////

    glGenFramebuffers(1, &internal::framebuffer.handle);
    glBindFramebuffer(GL_FRAMEBUFFER, internal::framebuffer.handle);

    // Generate off-screen texture.

    glGenTextures(1, &internal::framebuffer.texture);
    glBindTexture(GL_TEXTURE_2D, internal::framebuffer.texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _w, _h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, internal::framebuffer.texture, 0);

    internal::framebuffer.width = CAST(float, _w);
    internal::framebuffer.height = CAST(float, _h);

    // Generate renderbuffer.

    glGenRenderbuffers(1, &internal::framebuffer.rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, internal::framebuffer.rbo);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _w, _h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, internal::framebuffer.rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOG_ERROR(ERR_MAX, "Failed to complete framebuffer!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FILDEF void enable_framebuffer ()
{
    glBindFramebuffer(GL_FRAMEBUFFER, internal::framebuffer.handle);

    // By default draw to the entire framebuffer region.
    set_viewport(0, 0, internal::framebuffer.width, internal::framebuffer.height);

    if (internal::render_mode == RENDER_MODE_3D) {
        glEnable(GL_DEPTH_TEST);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    internal::framebuffer.enabled = true;
}

FILDEF void disable_framebuffer ()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    internal::framebuffer.enabled = false;

    set_viewport(0, 0, get_render_target_w(), get_render_target_h());
}

FILDEF void draw_framebuffer ()
{
    glBindTexture(GL_TEXTURE_2D, internal::framebuffer.texture);
    glBindVertexArray(internal::framebuffer.vao);

    set_shader(effect_catalog.GET_SHADER_RESOURCE(internal::framebuffer.effect));
    set_uniform_1i("framebuffer", GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

FILDEF void set_framebuffer_effect (std::string _shader)
{
    internal::framebuffer.effect = _shader;
}

FILDEF bool is_framebuffer_enabled ()
{
    return internal::framebuffer.enabled;
}

FILDEF float get_framebuffer_w ()
{
    return internal::framebuffer.width;
}
FILDEF float get_framebuffer_h ()
{
    return internal::framebuffer.height;
}

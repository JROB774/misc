namespace internal
{
    GLOBAL SDL_GLContext gl_context;
    GLOBAL Render_Mode  render_mode;
    GLOBAL Quad     render_viewport;
    GLOBAL Window*    render_target;

    GLOBAL Mat4 projection_matrix;
    GLOBAL Mat4 view_matrix;
}

#include "renderer_3d.cpp"
#include "renderer_2d.cpp"

FILDEF bool init_renderer ()
{
    // Initialise the renderer and it's GL context for use with our window.
    internal::gl_context = SDL_GL_CreateContext(get_window("WINMAIN").window);
    if (!internal::gl_context) {
        LOG_ERROR(ERR_MAX, "Failed to create GL context! (%s)", SDL_GetError());
        return false;
    }

    // Dynamically load the available OpenGL functions for rendering.
    // This needs to happen after we have created our SDL GL context.
    if (!GLL_load_gl_procs(GL_CONTEXT_VERSION_MAJOR, GL_CONTEXT_VERSION_MINOR, GLL_LOAD_CORE_PROFILE, SDL_GL_GetProcAddress)) {
        LOG_ERROR(ERR_MAX, "Failed to load GL%d.%d procedures!", GL_CONTEXT_VERSION_MAJOR, GL_CONTEXT_VERSION_MINOR);
        return false;
    }

    // DUMP DEBUG INFO //////////////////////////////////////////////
    LOG_DEBUG("Initialized OpenGL Renderer");
    const GLubyte* gl_version   = glGetString(GL_VERSION);
    const GLubyte* gl_slversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const GLubyte* gl_renderer  = glGetString(GL_RENDERER);
    const GLubyte* gl_vendor    = glGetString(GL_VENDOR);

    GLint max_texture_size;
    GLint max_texture_units;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units); // Minimum will be 16.

    begin_debug_section("OpenGL:");
    LOG_DEBUG("Version %s", gl_version);
    LOG_DEBUG("GLSL Version %s", gl_slversion);
    LOG_DEBUG("Renderer: %s", gl_renderer);
    LOG_DEBUG("Vendor: %s", gl_vendor);
    LOG_DEBUG("Max Texture Size: %d", max_texture_size);
    LOG_DEBUG("Max Texture Units: %d", max_texture_units);
    end_debug_section();
    // DUMP DEBUG INFO //////////////////////////////////////////////

    set_render_target("WINMAIN");

    set_render_mode(RENDER_MODE_3D); // Default is 3D rendering.
    set_viewport(0, 0, get_render_target_w(), get_render_target_h());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    init_renderer_3d();
    init_renderer_2d();

    return true;
}

FILDEF void quit_renderer ()
{
    quit_renderer_2d();
    quit_renderer_3d();

    SDL_GL_DeleteContext(internal::gl_context);
    internal::gl_context = NULL;
}

FILDEF void set_render_mode (Render_Mode _mode)
{
    internal::render_mode = _mode;

    switch (internal::render_mode) {
    case (RENDER_MODE_2D): { enable_renderer_2d(); } break;
    case (RENDER_MODE_3D): { enable_renderer_3d(); } break;
    }
}

FILDEF void render_clear (float _r, float _g, float _b, float _a)
{
    glClearColor(_r, _g, _b, _a);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
FILDEF void render_clear (Vec4 _clear)
{
    render_clear(_clear.r, _clear.g, _clear.b, _clear.a);
}

FILDEF void render_present ()
{
    SDL_GL_SwapWindow(internal::render_target->window);
}

FILDEF void set_render_target (std::string _name)
{
    internal::render_target = &get_window(_name);
    ASSERT(internal::render_target);

    if (SDL_GL_MakeCurrent(internal::render_target->window, internal::gl_context) < 0) {
        internal::render_target = NULL;
        LOG_ERROR(ERR_MED, "Failed to set render target! (%s)", SDL_GetError());
    }
}
FILDEF Window* get_render_target ()
{
    return internal::render_target;
}

FILDEF float get_render_target_w ()
{
    if (is_framebuffer_enabled()) { return get_framebuffer_w(); }
    int w = 0;
    SDL_GL_GetDrawableSize(internal::render_target->window, &w, NULL);
    return CAST(float, w);
}

FILDEF float get_render_target_h ()
{
    if (is_framebuffer_enabled()) { return get_framebuffer_h(); }
    int h = 0;
    SDL_GL_GetDrawableSize(internal::render_target->window, NULL, &h);
    return CAST(float, h);
}

STDDEF void set_viewport (float _x, float _y, float _w, float _h)
{
    internal::render_viewport = { _x, _y, _w, _h };
    Quad v = internal::render_viewport;

    // Converts a viewport in our 2D top-left format to GL's bottom-left format.
    if (internal::render_mode == RENDER_MODE_2D) {
        v = internal::convert_viewport(internal::render_viewport);
        set_projection(glm::ortho(0.0f, _w, _h, 0.0f));
    }

    glViewport(CAST(GLint, v.x), CAST(GLint, v.y), CAST(GLsizei, v.w), CAST(GLsizei, v.h));
}

FILDEF void set_viewport (Quad _v)
{
    set_viewport(_v.x, _v.y, _v.w, _v.h);
}

FILDEF Quad get_viewport ()
{
    return internal::render_viewport;
}

STDDEF void set_projection (Mat4 _matrix)
{
    internal::projection_matrix = _matrix;
}
STDDEF Mat4 get_projection ()
{
    return internal::projection_matrix;
}

STDDEF void set_view (Mat4 _matrix)
{
    internal::view_matrix = _matrix;
}
STDDEF Mat4 get_view ()
{
    return internal::view_matrix;
}

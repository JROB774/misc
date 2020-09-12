namespace internal
{
    GLOBAL Vec4 primitive_color;
    GLOBAL VAO  primitive_vao;
    GLOBAL VBO  primitive_vbo;

    struct PolygonVertex
    {
        Vec2 pos;
        Vec4 color;
    };

    std::vector<PolygonVertex> polygon_verts;

    GLOBAL VAO polygon_vao;
    GLOBAL VBO polygon_vbo;

    GLOBAL float font_draw_scale;
    GLOBAL Vec2 texture_draw_scale;

    // Highly unlikely we will ever need more embedded scissors than this but if we
    // do we will know about it through various code ASSERTS and can just increase.
    GLOBAL Stack<Quad, 256> scissor_stack;

    FILDEF Quad convert_viewport (Quad _v)
    {
        return { _v.x, (get_render_target_h() - (_v.y + _v.h)), _v.w, _v.h };
    }
}

FILDEF void init_renderer_2d ()
{
    glGenVertexArrays(1, &internal::primitive_vao);
    glGenBuffers(1, &internal::primitive_vbo);

    glBindVertexArray(internal::primitive_vao);
    glBindBuffer(GL_ARRAY_BUFFER, internal::primitive_vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)(0*sizeof(GLfloat)));
    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // TexCoord

    glGenVertexArrays(1, &internal::polygon_vao);
    glGenBuffers(1, &internal::polygon_vbo);

    glBindVertexArray(internal::polygon_vao);
    glBindBuffer(GL_ARRAY_BUFFER, internal::polygon_vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(internal::PolygonVertex), (void*)(0*sizeof(GLfloat)));
    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(internal::PolygonVertex), (void*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // Color

    internal::primitive_color     = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    internal::font_draw_scale     = 1.0f;
    internal::texture_draw_scale   = Vec2(1.0f, 1.0f);
    internal::scissor_stack.count = 0;
}

FILDEF void quit_renderer_2d ()
{
    glDeleteVertexArrays(1, &internal::primitive_vao);
    glDeleteBuffers(1, &internal::primitive_vbo);
}

FILDEF void enable_renderer_2d ()
{
    // Modify the current viewport to be in our 2D coorindate space.
    set_viewport(internal::render_viewport);
    glDisable(GL_DEPTH_TEST);

    set_projection(glm::ortho(0.0f, internal::render_viewport.w, internal::render_viewport.h, 0.0f));
    set_view(Mat4(1));
}

STDDEF Vec2 screen_to_world (Vec2 _screen)
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    // GL expects bottom-left so we have to convert our viewport first.
    Quad v = internal::convert_viewport(internal::render_viewport);

    // We also need to do flip the Y coordinate to use this system.
    _screen.y = get_render_target_h() - _screen.y;
    Vec3 coord(_screen.x, _screen.y, 0.0f);

    Mat4 projection = internal::projection_matrix;
    Mat4 modelview = internal::view_matrix;
    Vec4 viewport = Vec4(v.x, v.y, v.w, v.h);

    coord = glm::unProject(coord, modelview, projection, viewport);
    Vec2 world = Vec2(coord.x, coord.y);

    return world;
}

STDDEF Vec2 world_to_screen (Vec2 _world)
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    // GL expects bottom-left so we have to convert our viewport first.
    Quad v = internal::convert_viewport(internal::render_viewport);

    Vec3 coord(_world.x, _world.y, 0.0f);

    Mat4 projection = internal::projection_matrix;
    Mat4 modelview = internal::view_matrix;
    Vec4 viewport = Vec4(v.x, v.y, v.w, v.h);

    coord = glm::project(coord, modelview, projection, viewport);
    Vec2 screen = Vec2(coord.x, coord.y);

    // We also need to do flip the Y coordinate to use this system.
    screen.x -= v.x;
    screen.y  = get_render_target_h() - (screen.y + internal::render_viewport.y);

    return screen;
}

FILDEF void set_texture_draw_scale (float _sx, float _sy)
{
    internal::texture_draw_scale = Vec2(_sx, _sy);
}
FILDEF Vec2 get_texture_draw_scale ()
{
    return internal::texture_draw_scale;
}

FILDEF void set_font_draw_scale (float _scale)
{
    internal::font_draw_scale = _scale;
}
FILDEF float get_font_draw_scale ()
{
    return internal::font_draw_scale;
}

FILDEF void set_draw_color (float _r, float _g, float _b, float _a)
{
    internal::primitive_color = Vec4(_r, _g, _b, _a);
}
FILDEF void set_draw_color (Vec4 _color)
{
    internal::primitive_color = _color;
}

FILDEF void set_line_width (float _width)
{
    glLineWidth(_width);
}

FILDEF void set_point_size (float _size)
{
    glPointSize(_size);
}

FILDEF void draw_point (float _x, float _y)
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    set_shader(shader_catalog.GET_SHADER_RESOURCE("2DBASIC"));

    set_uniform_mat4("projection", internal::projection_matrix);
    set_uniform_mat4("view", internal::view_matrix);

    set_uniform_vec4("color", internal::primitive_color);

    GLfloat verts[] = { _x, _y, 0.0f, 0.0f };

    glBindVertexArray(internal::primitive_vao);

    glBindBuffer(GL_ARRAY_BUFFER, internal::primitive_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_POINTS, 0, 1);
}

FILDEF void draw_line (float _x1, float _y1, float _x2, float _y2)
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    set_shader(shader_catalog.GET_SHADER_RESOURCE("2DBASIC"));

    set_uniform_mat4("projection", internal::projection_matrix);
    set_uniform_mat4("view", internal::view_matrix);

    set_uniform_vec4("color", internal::primitive_color);

    GLfloat verts[]
    {
    _x1, _y1, 0.0f, 0.0f,
    _x2, _y2, 0.0f, 0.0f,
    };

    glBindVertexArray(internal::primitive_vao);

    glBindBuffer(GL_ARRAY_BUFFER, internal::primitive_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_LINES, 0, 2);
}

FILDEF void draw_quad (float _x1, float _y1, float _x2, float _y2)
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    set_shader(shader_catalog.GET_SHADER_RESOURCE("2DBASIC"));

    set_uniform_mat4("projection", internal::projection_matrix);
    set_uniform_mat4("view", internal::view_matrix);
    set_uniform_vec4("color", internal::primitive_color);

    // We want to use 0.5 otherwise the lines don't draw where we want them.
    _x1 += 0.5f;
    _y1 += 0.5f;
    _x2 -= 0.5f;
    _y2 -= 0.5f;

    GLfloat verts[]
    {
    _x1, _y1, 0.0f, 0.0f,
    _x2, _y1, 0.0f, 0.0f,
    _x2, _y2, 0.0f, 0.0f,
    _x1, _y2, 0.0f, 0.0f
    };

    glBindVertexArray(internal::primitive_vao);

    glBindBuffer(GL_ARRAY_BUFFER, internal::primitive_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

FILDEF void fill_quad (float _x1, float _y1, float _x2, float _y2)
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    set_shader(shader_catalog.GET_SHADER_RESOURCE("2DBASIC"));

    set_uniform_mat4("projection", internal::projection_matrix);
    set_uniform_mat4("view", internal::view_matrix);
    set_uniform_vec4("color", internal::primitive_color);

    GLfloat verts[]
    {
    _x1, _y2, 0.0f, 0.0f,
    _x1, _y1, 0.0f, 0.0f,
    _x2, _y2, 0.0f, 0.0f,
    _x2, _y1, 0.0f, 0.0f
    };

    glBindVertexArray(internal::primitive_vao);

    glBindBuffer(GL_ARRAY_BUFFER, internal::primitive_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

STDDEF void draw_texture (const Texture& _tex, float _x, float _y, const Quad* _clip)
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    set_shader(shader_catalog.GET_SHADER_RESOURCE("2DIMAGE"));

    set_uniform_mat4("projection", internal::projection_matrix);
    set_uniform_mat4("view", internal::view_matrix);
    set_uniform_vec4("color", _tex.color);
    set_uniform_1i  ("texture0", GL_TEXTURE0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _tex.handle);

    float cx1, cy1, cx2, cy2;
    float w, h;

    if (_clip) {
        cx1 =       (_clip->x / _tex.w);
        cy1 =       (_clip->y / _tex.h);
        cx2 = cx1 + (_clip->w / _tex.w);
        cy2 = cy1 + (_clip->h / _tex.h);

        w = _clip->w * internal::texture_draw_scale.x;
        h = _clip->h * internal::texture_draw_scale.y;
    } else {
        cx1 = 0.0f;
        cy1 = 0.0f;
        cx2 = 1.0f;
        cy2 = 1.0f;

        w = _tex.w * internal::texture_draw_scale.x;
        h = _tex.h * internal::texture_draw_scale.y;
    }

    float x1 = _x - (w / 2.0f); // Center anchor.
    float y1 = _y - (h / 2.0f); // Center anchor.
    float x2 = x1 + w;
    float y2 = y1 + h;

    GLfloat verts[]
    {
    x1, y2, cx1, cy2,
    x1, y1, cx1, cy1,
    x2, y2, cx2, cy2,
    x2, y1, cx2, cy1
    };

    glBindVertexArray(internal::primitive_vao);

    glBindBuffer(GL_ARRAY_BUFFER, internal::primitive_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

STDDEF void draw_text (const Truetype& _font, float _x, float _y, const char* _text)
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    const Truetype_Face& font = (_font.mode == TRUETYPE_MODE_SMALL) ? _font.sm : _font.lg;

    set_shader(shader_catalog.GET_SHADER_RESOURCE("2DTEXT"));

    set_uniform_mat4("projection", internal::projection_matrix);
    set_uniform_mat4("view", internal::view_matrix);
    set_uniform_vec4("color", font.color);
    set_uniform_1i  ("texture0", GL_TEXTURE0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font.cache.handle);

    glBindVertexArray(internal::primitive_vao);
    glBindBuffer(GL_ARRAY_BUFFER, internal::primitive_vbo);

    int index      = 0;
    int prev_index = 0;

    float x = _x;
    float y = _y;

    float scale = internal::font_draw_scale;

    for (const char* c=_text; *c; ++c) {
        x += (get_truetype_kerning(_font, *c, index, prev_index) * scale);
        switch (*c) {
        case ('\n'): { x = _x, y -= (font.line_gap * scale); } break;
        case ('\t'): { x += (get_truetype_tab_width(_font) * scale); } break;
        default: {
            const Truetype_Glyph& glyph = font.glyphs[*c];
            const Quad& clip = glyph.bounds;

            float bearing_x = glyph.bearing.x * scale;
            float bearing_y = glyph.bearing.y * scale;

            float cx1 =       ((clip.x / font.cache.w));
            float cy1 =       ((clip.y / font.cache.h));
            float cx2 = cx1 + ((clip.w / font.cache.w));
            float cy2 = cy1 + ((clip.h / font.cache.h));

            float w = clip.w * scale;
            float h = clip.h * scale;

            float x1 = x  + bearing_x;
            float y1 = y  - bearing_y;
            float x2 = x1 + w;
            float y2 = y1 + h;

            GLfloat verts[]
            {
            x1, y2, cx1, cy2,
            x1, y1, cx1, cy1,
            x2, y2, cx2, cy2,
            x2, y1, cx2, cy1
            };

            glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            x += (glyph.advance * scale);
        } break;
        }
    }
}

STDDEF void begin_scissor (float _x, float _y, float _w, float _h)
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    // Our version of setting the scissor region takes the currently set
    // viewport into consideration rather than basing the region on the
    // size of the screen/window. As a result, we need to perform a few
    // operations on the values passed in to make sure this the case.

    // We push scissor regions onto a stack so we can stack scissor calls.
    // This is particularly useful for the GUI which uses many scissors.
    if (internal::scissor_stack.count == 0) { glEnable(GL_SCISSOR_TEST); }
    internal::scissor_stack.push({ _x, _y, _w, _h });

    // GL expects bottom-left so we have to flip the Y coordinate around.
    GLint   x = CAST(GLint,   internal::render_viewport.x + _x);
    GLint   y = CAST(GLint,   get_render_target_h() - (internal::render_viewport.y + (_y + _h)));
    GLsizei w = CAST(GLsizei, _w);
    GLsizei h = CAST(GLsizei, _h);

    glScissor(x, y, w, h);
}
STDDEF void end_scissor ()
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    // Pop the last scissor region off the stack.
    Quad s = internal::scissor_stack.pop();

    // GL expects bottom-left so we have to flip the Y coordinate around.
    GLint   x = CAST(GLint,   internal::render_viewport.x + s.x);
    GLint   y = CAST(GLint,   get_render_target_h() - (internal::render_viewport.y + (s.y + s.h)));
    GLsizei w = CAST(GLsizei, s.w);
    GLsizei h = CAST(GLsizei, s.h);

    glScissor(x, y, w, h);

    if (internal::scissor_stack.count == 0) { glDisable(GL_SCISSOR_TEST); }
}

FILDEF void begin_stencil ()
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    glEnable(GL_STENCIL_TEST);

    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
}
FILDEF void end_stencil ()
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    stencil_mode_draw();
    glDisable(GL_STENCIL_TEST);
}
FILDEF void stencil_mode_erase ()
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_ALWAYS, CAST(GLuint, 1), CAST(GLuint, ~0));
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}
FILDEF void stencil_mode_draw ()
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_NOTEQUAL, CAST(GLuint, 1), CAST(GLuint, ~0));
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

FILDEF void begin_polygon ()
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);
    internal::polygon_verts.clear();
}
FILDEF void end_polygon ()
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    set_shader(shader_catalog.GET_SHADER_RESOURCE("2DPOLY"));

    set_uniform_mat4("projection", internal::projection_matrix);
    set_uniform_mat4("view", internal::view_matrix);

    glBindVertexArray(internal::polygon_vao);

    glBindBuffer(GL_ARRAY_BUFFER, internal::polygon_vbo);
    glBufferData(GL_ARRAY_BUFFER, internal::polygon_verts.size()*sizeof(internal::PolygonVertex), &internal::polygon_verts[0], GL_STREAM_DRAW);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, CAST(GLsizei, internal::polygon_verts.size()));
}
FILDEF void put_vertex (float _x, float _y, Vec4 _color)
{
    ASSERT(internal::render_mode == RENDER_MODE_2D);

    internal::polygon_verts.push_back({ Vec2(_x, _y), _color });
}

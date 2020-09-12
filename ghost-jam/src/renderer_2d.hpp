FILDEF void init_renderer_2d ();
FILDEF void quit_renderer_2d ();

FILDEF void enable_renderer_2d ();

STDDEF Vec2 screen_to_world (Vec2 _screen);
STDDEF Vec2 world_to_screen (Vec2 _world);

FILDEF void  set_font_draw_scale    (float _scale);
FILDEF float get_font_draw_scale    ();

// STUFF YOU PROBS CARE ABOUT //////////////////////////////////////////////////

FILDEF void set_texture_draw_scale (float _sx, float _sy);
FILDEF Vec2 get_texture_draw_scale ();

FILDEF void set_draw_color (float _r, float _g, float _b, float _a=1.0f);
FILDEF void set_draw_color (Vec4 _color);

FILDEF void set_line_width (float _width);
FILDEF void set_point_size (float _size);

FILDEF void draw_point (float _x,  float _y);
FILDEF void draw_line  (float _x1, float _y1, float _x2, float _y2);
FILDEF void draw_quad  (float _x1, float _y1, float _x2, float _y2);
FILDEF void fill_quad  (float _x1, float _y1, float _x2, float _y2);

STDDEF void draw_texture (const Texture& _tex, float _x, float _y, const Quad* _clip = NULL);
STDDEF void draw_text    (const Truetype& _font, float _x, float _y, std::string _text);

STDDEF void begin_scissor (float _x, float _y, float _w, float _h);
STDDEF void end_scissor   ();

FILDEF void begin_stencil      ();
FILDEF void end_stencil        ();
FILDEF void stencil_mode_erase ();
FILDEF void stencil_mode_draw  ();

FILDEF void begin_polygon ();
FILDEF void end_polygon   ();
FILDEF void put_vertex    (float _x, float _y, Vec4 _color);

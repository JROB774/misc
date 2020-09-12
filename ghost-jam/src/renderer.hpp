GLOBAL constexpr int GL_CONTEXT_VERSION_MAJOR = 3;
GLOBAL constexpr int GL_CONTEXT_VERSION_MINOR = 3;

enum Render_Mode
{
    RENDER_MODE_3D,
    RENDER_MODE_2D
};

FILDEF bool init_renderer   ();
FILDEF void quit_renderer   ();

FILDEF void set_render_mode (Render_Mode _mode);

FILDEF void render_clear    (float _r, float _g, float _b, float _a);
FILDEF void render_clear    (Vec4 _clear);
FILDEF void render_present  ();

FILDEF void    set_render_target (std::string _name);
FILDEF Window* get_render_target ();

FILDEF float get_render_target_w ();
FILDEF float get_render_target_h ();

STDDEF void set_viewport (float _x, float _y, float _w, float _h);
FILDEF void set_viewport (Quad _v);
FILDEF Quad get_viewport ();

STDDEF void set_projection (Mat4 _matrix);
STDDEF Mat4 get_projection ();
STDDEF void set_view       (Mat4 _matrix);
STDDEF Mat4 get_view       ();

#include "renderer_3d.hpp"
#include "renderer_2d.hpp"

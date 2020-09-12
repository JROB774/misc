FILDEF void generate_framebuffer   (int _w, int _h);
FILDEF void destroy_framebuffer    ();
FILDEF void resize_framebuffer     (int _w, int _h);
FILDEF void enable_framebuffer     ();
FILDEF void disable_framebuffer    ();
FILDEF void draw_framebuffer       ();
FILDEF void set_framebuffer_effect (std::string _shader);
FILDEF bool is_framebuffer_enabled ();

FILDEF float get_framebuffer_w ();
FILDEF float get_framebuffer_h ();

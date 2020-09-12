// @Improve: Make these private and add functions for drawing particular 3d primitives...
GLOBAL Mesh cube_mesh;
GLOBAL Mesh plane_mesh;
GLOBAL Mesh sprite_mesh;

FILDEF void init_renderer_3d ();
FILDEF void quit_renderer_3d ();

FILDEF void enable_renderer_3d ();

FILDEF void draw_mesh (const Mesh& _mesh, Mat4 _model);

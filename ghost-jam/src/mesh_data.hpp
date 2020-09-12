// Just adds some faux-lighting to darken certain sides adding a bit more depth.
GLOBAL const Vec4 MESH_L_COLOR = { 1.0f,1.0f,1.0f,1.0f };
GLOBAL const Vec4 MESH_D_COLOR = { 1.0f,1.0f,1.0f,1.0f };

GLOBAL const Vertex CUBE_MESH[] =
{
// BACK
{{ -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  0.0f,  1.0f } }, // BL
{{  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  0.0f,  1.0f } }, // TR
{{  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  0.0f,  1.0f } }, // BR
{{  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  0.0f,  1.0f } }, // TR
{{ -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  0.0f,  1.0f } }, // BL
{{ -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  0.0f,  1.0f } }, // TL
// FRONT
{{ -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // BL
{{  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // BR
{{  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // TR
{{  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // TR
{{ -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // TL
{{ -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // BL
// LEFT
{{ -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, MESH_D_COLOR, {  1.0f,  0.0f,  0.0f } }, // TR
{{ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, MESH_D_COLOR, {  1.0f,  0.0f,  0.0f } }, // TL
{{ -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, MESH_D_COLOR, {  1.0f,  0.0f,  0.0f } }, // BL
{{ -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, MESH_D_COLOR, {  1.0f,  0.0f,  0.0f } }, // BL
{{ -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f }, MESH_D_COLOR, {  1.0f,  0.0f,  0.0f } }, // BR
{{ -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, MESH_D_COLOR, {  1.0f,  0.0f,  0.0f } }, // TR
// RIGHT
{{  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f }, MESH_D_COLOR, { -1.0f,  0.0f,  0.0f } }, // TL
{{  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f }, MESH_D_COLOR, { -1.0f,  0.0f,  0.0f } }, // BR
{{  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, MESH_D_COLOR, { -1.0f,  0.0f,  0.0f } }, // TR
{{  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f }, MESH_D_COLOR, { -1.0f,  0.0f,  0.0f } }, // BR
{{  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f }, MESH_D_COLOR, { -1.0f,  0.0f,  0.0f } }, // TL
{{  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, MESH_D_COLOR, { -1.0f,  0.0f,  0.0f } }, // BL
// BOTTOM
{{ -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  1.0f,  0.0f } }, // TR
{{  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  1.0f,  0.0f } }, // TL
{{  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  1.0f,  0.0f } }, // BL
{{  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  1.0f,  0.0f } }, // BL
{{ -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  1.0f,  0.0f } }, // BR
{{ -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  1.0f,  0.0f } }, // TR
// TOP
{{ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }, // TL
{{  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }, // BR
{{  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }, // TR
{{  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }, // BR
{{ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }, // TL
{{ -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }  // BL
};

GLOBAL const Vertex PLANE_MESH[] =
{
{{ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }, // TL
{{  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }, // BR
{{  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }, // TR
{{  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }, // BR
{{ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }, // TL
{{ -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f }, MESH_L_COLOR, {  0.0f, -1.0f,  0.0f } }  // BL
};

GLOBAL const Vertex SPRITE_MESH[] =
{
{{ -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // BL
{{  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // BR
{{  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // TR
{{  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // TR
{{ -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // TL
{{ -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, MESH_L_COLOR, {  0.0f,  0.0f, -1.0f } }, // BL
};

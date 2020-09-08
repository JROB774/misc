/*******************************************************************************
 * TEXTURED
 * Draws a texture over a 2D polygonal mesh to the screen -- with modulation.
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

precision mediump float;

attribute vec2 in_vert_pos;
attribute vec2 in_tex_coord;

uniform mat3 world_space_matrix;
uniform mat3 object_space_matrix;

varying vec2 tex_coord;

void main ()
{
    gl_Position = vec4(world_space_matrix * object_space_matrix * vec3(in_vert_pos, 1.0), 1.0);
    tex_coord = in_tex_coord;
}

/*////////////////////////////////////////////////////////////////////////////*/

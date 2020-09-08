/*******************************************************************************
 * UNTEXTURED
 * Draws untextured 2D polygonal meshes to the screen in a desired color.
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

precision mediump float;

attribute vec2 in_vert_pos;

uniform mat3 world_space_matrix;
uniform vec4 object_draw_color;

varying vec4 vert_color;

void main ()
{
    gl_Position = vec4(world_space_matrix * vec3(in_vert_pos, 1.0), 1.0);
    vert_color = object_draw_color;
}

/*////////////////////////////////////////////////////////////////////////////*/

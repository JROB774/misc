/*******************************************************************************
 * BASIC
 * Post-processing shader that outputs the framebuffer as is, with no effects.
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

precision mediump float;

attribute vec2 in_vert_pos;
attribute vec2 in_tex_coord;

varying vec2 tex_coord;

void main ()
{
    gl_Position = vec4(vec3(in_vert_pos, 1.0), 1.0);
    tex_coord = in_tex_coord;
}

/*////////////////////////////////////////////////////////////////////////////*/

/*******************************************************************************
 * TEXTURED
 * Draws a texture over a 2D polygonal mesh to the screen -- with modulation.
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

precision mediump float;

uniform sampler2D texture;
uniform vec4 mod_color;

varying vec2 tex_coord;

void main ()
{
    gl_FragColor = texture2D(texture, tex_coord) * mod_color;
}

/*////////////////////////////////////////////////////////////////////////////*/

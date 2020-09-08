/*******************************************************************************
 * BASIC
 * Post-processing shader that outputs the framebuffer as is, with no effects.
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

precision mediump float;

uniform sampler2D framebuffer;
varying vec2 tex_coord;

void main ()
{
    gl_FragColor = texture2D(framebuffer, tex_coord);
}

/*////////////////////////////////////////////////////////////////////////////*/

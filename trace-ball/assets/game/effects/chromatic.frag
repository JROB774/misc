/*******************************************************************************
 * CHROMATIC
 * Post-processing shader that applies chromatic aberration to the framebuffer.
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

precision mediump float;

uniform sampler2D framebuffer;
uniform vec2 pulse;

varying vec2 tex_coord;

void main ()
{
    vec4 out_color = vec4(1.0, 1.0, 1.0, 1.0);

    out_color.r = texture2D(framebuffer, tex_coord - vec2( pulse.x,          0.0)).r;
    out_color.g = texture2D(framebuffer, tex_coord - vec2(         0.0, -pulse.y)).g;
    out_color.b = texture2D(framebuffer, tex_coord - vec2(-pulse.x,          0.0)).b;
    out_color.a = texture2D(framebuffer, tex_coord                               ).a;

    gl_FragColor = out_color;
}

/*////////////////////////////////////////////////////////////////////////////*/

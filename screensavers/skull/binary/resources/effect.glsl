uniform sampler2D texture0;
uniform mat4    projection;
uniform vec2        random;

#if COMPILING_VERT_SHADER /*//////////////////////////////////////////////////*/

layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec2 in_texcoord;

out vec2 texcoord;

void main ()
{
    gl_Position = projection * vec4(in_pos,0,1);
    texcoord = in_texcoord;
}

#endif /* COMPILING_VERT_SHADER //////////////////////////////////////////////*/

#if COMPILING_FRAG_SHADER /*//////////////////////////////////////////////////*/

in  vec2 texcoord;
out vec4 frag_color;

void main ()
{
    frag_color.r = texture(texture0, texcoord - vec2( random.x,       0.0)).r;
    frag_color.g = texture(texture0, texcoord - vec2(      0.0, -random.y)).g;
    frag_color.b = texture(texture0, texcoord - vec2(-random.x,       0.0)).b;
    frag_color.a = texture(texture0, texcoord).a;
}

#endif /* COMPILING_FRAG_SHADER //////////////////////////////////////////////*/

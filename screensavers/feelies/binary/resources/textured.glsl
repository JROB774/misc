uniform sampler2D texture0;
uniform mat4    projection;
uniform mat4     modelview;
uniform vec2    tex_offset;

#if COMPILING_VERT_SHADER /*//////////////////////////////////////////////////*/

layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec2 in_texcoord;

out vec2 texcoord;

void main ()
{
    gl_Position = projection * modelview * vec4(in_pos,0,1);
    texcoord = in_texcoord;
}

#endif /* COMPILING_VERT_SHADER //////////////////////////////////////////////*/

#if COMPILING_FRAG_SHADER /*//////////////////////////////////////////////////*/

in  vec2 texcoord;
out vec4 frag_color;

void main ()
{
    frag_color = texture(texture0, texcoord+tex_offset);
}

#endif /* COMPILING_FRAG_SHADER //////////////////////////////////////////////*/

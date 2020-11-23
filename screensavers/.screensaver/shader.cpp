/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

STDDEF GLuint impl__compile_shader (const GLchar* src, GLint len, GLenum type)
{
    GLuint shader = glCreateShader(type);

    const GLchar* sources[2] = {};
    GLint source_lengths[2] = {};

    sources[1] = src;

    if (type == GL_FRAGMENT_SHADER)
    {
        sources[0] = "#version 330\n#define COMPILING_FRAG_SHADER 1\n";
    }
    if (type == GL_VERTEX_SHADER)
    {
        sources[0] = "#version 330\n#define COMPILING_VERT_SHADER 1\n";
    }

    source_lengths[0] = CAST(GLint, cstr_length(sources[0]));
    source_lengths[1] = len;

    glShaderSource(shader, 2, &sources[0], &source_lengths[0]);

    glCompileShader(shader);

    GLint compile_success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_success);
    if (!compile_success)
    {
        glDeleteShader(shader);
        shader = NULL;
    }

    return shader;
}

STDDEF bool impl__create_shader (Shader& shader, const void* buf, size_t sz)
{
    const GLchar* src = CAST(const GLchar*, buf);
    GLint src_length = CAST(GLint, sz);

    GLuint vert = impl__compile_shader(src, src_length, GL_VERTEX_SHADER  );
    GLuint frag = impl__compile_shader(src, src_length, GL_FRAGMENT_SHADER);

    defer { glDeleteShader(vert); };
    defer { glDeleteShader(frag); };

    if (!vert || !frag) return false;

    shader = glCreateProgram();
    if (!shader) return false;

    glAttachShader(shader, vert);
    glAttachShader(shader, frag);

    glLinkProgram(shader);

    GLint link_success;
    glGetProgramiv(shader, GL_LINK_STATUS, &link_success);
    if (!link_success)
    {
        glDeleteProgram(shader);
        return false;
    }

    return true;
}

/* -------------------------------------------------------------------------- */

STDDEF bool load_shader_from_file (Shader& shader, const char* file_name)
{
    File_Content file_content = read_entire_file_content(file_name);
    if (!file_content.data || !file_content.size)
    {
        return false;
    }
    defer { free_file_content(&file_content); };

    return impl__create_shader(shader, file_content.data, file_content.size);
}

STDDEF bool load_shader_from_mem (Shader& shader, const void* buf, size_t sz)
{
    return impl__create_shader(shader, buf, sz);
}

STDDEF void free_shader (Shader& shader)
{
    glDeleteProgram(shader);
}

FILDEF void use_shader (Shader& shader)
{
    glUseProgram(shader);
}

/* -------------------------------------------------------------------------- */

FILDEF void set_uniform_1f (Shader& shader, const char* name, float a)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform1f(loc, a);
}
FILDEF void set_uniform_2f (Shader& shader, const char* name, float a, float b)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform2f(loc, a, b);
}
FILDEF void set_uniform_3f (Shader& shader, const char* name, float a, float b, float c)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform3f(loc, a, b, c);
}
FILDEF void set_uniform_4f (Shader& shader, const char* name, float a, float b, float c, float d)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform4f(loc, a, b, c, d);
}
FILDEF void set_uniform_1i (Shader& shader, const char* name, int a)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform1i(loc, a);
}
FILDEF void set_uniform_2i (Shader& shader, const char* name, int a, int b)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform2i(loc, a, b);
}
FILDEF void set_uniform_3i (Shader& shader, const char* name, int a, int b, int c)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform3i(loc, a, b, c);
}
FILDEF void set_uniform_4i (Shader& shader, const char* name, int a, int b, int c, int d)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform4i(loc, a, b, c, d);
}
FILDEF void set_uniform_vec2 (Shader& shader, const char* name, vec2 vec)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform2f(loc, vec.x, vec.y);
}
FILDEF void set_uniform_vec3 (Shader& shader, const char* name, vec3 vec)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform3f(loc, vec.x, vec.y, vec.z);
}
FILDEF void set_uniform_vec4 (Shader& shader, const char* name, vec4 vec)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}
FILDEF void set_uniform_1fv (Shader& shader, const char* name, const float* val)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform1fv(loc, 1, val);
}
FILDEF void set_uniform_2fv (Shader& shader, const char* name, const float* val)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform2fv(loc, 2, val);
}
FILDEF void set_uniform_3fv (Shader& shader, const char* name, const float* val)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform3fv(loc, 3, val);
}
FILDEF void set_uniform_4fv (Shader& shader, const char* name, const float* val)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform4fv(loc, 4, val);
}
FILDEF void set_uniform_1iv (Shader& shader, const char* name, const int* val)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform1iv(loc, 1, val);
}
FILDEF void set_uniform_2iv (Shader& shader, const char* name, const int* val)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform2iv(loc, 2, val);
}
FILDEF void set_uniform_3iv (Shader& shader, const char* name, const int* val)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform3iv(loc, 3, val);
}
FILDEF void set_uniform_4iv (Shader& shader, const char* name, const int* val)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniform4iv(loc, 4, val);
}
FILDEF void set_uniform_mat2fv (Shader& shader, const char* name, int count, bool transpose, const float* val)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniformMatrix2fv(loc, count, transpose, val);
}
FILDEF void set_uniform_mat3fv (Shader& shader, const char* name, int count, bool transpose, const float* val)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniformMatrix3fv(loc, count, transpose, val);
}
FILDEF void set_uniform_mat4fv (Shader& shader, const char* name, int count, bool transpose, const float* val)
{
    GLint loc = glGetUniformLocation(shader, name);
    glUniformMatrix4fv(loc, count, transpose, val);
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

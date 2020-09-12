namespace internal
{
    GLOBAL Shader current_shader;

    FILDEF GLuint compile_shader (const GLchar* _source, GLenum _type)
    {
        GLuint shader = glCreateShader(_type);
        glShaderSource(shader, 1, &_source, NULL);
        glCompileShader(shader);

        // Determine the info log length and then temp allocate a buffer.
        GLint info_log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
        char* info_log = cstd_malloc(char, info_log_length);
        if (info_log) {
            defer { cstd_free(info_log); };

            GLint compile_success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_success);
            if (!compile_success) {
                glGetShaderInfoLog(shader, info_log_length, NULL, info_log);
                LOG_ERROR(ERR_MIN, "Failed to compile shader:\n%s", info_log);
            }
        }

        return shader;
    }
}

STDDEF Shader load_shader_from_source (const char* _source)
{
    std::string vert_src = _source;
    std::string frag_src = _source;

    string_replace(vert_src, " vert()", " main()");
    string_replace(frag_src, " frag()", " main()");

    string_replace(vert_src, "COMPILING_VERTEX_PROGRAM",   "1");
    string_replace(vert_src, "COMPILING_FRAGMENT_PROGRAM", "0");

    string_replace(frag_src, "COMPILING_VERTEX_PROGRAM",   "0");
    string_replace(frag_src, "COMPILING_FRAGMENT_PROGRAM", "1");

    const GLchar* vsrc = CAST(const GLchar*, vert_src.c_str());
    const GLchar* fsrc = CAST(const GLchar*, frag_src.c_str());

    GLuint vert = internal::compile_shader(vsrc, GL_VERTEX_SHADER);
    GLuint frag = internal::compile_shader(fsrc, GL_FRAGMENT_SHADER);

    defer { glDeleteShader(vert); glDeleteShader(frag); };

    // If either of the shaders failed to compile we return failure.
    if (!vert) { return NULL; }
    if (!frag) { return NULL; }

    Shader program = glCreateProgram();

    glAttachShader(program, vert);
    glAttachShader(program, frag);

    glLinkProgram(program);

    // Determine the info log length and then temp allocate a buffer.
    GLint info_log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
    char* info_log = cstd_malloc(char, info_log_length);
    if (info_log) {
        defer { cstd_free(info_log); };

        GLint link_success;
        glGetProgramiv(program, GL_LINK_STATUS, &link_success);
        if (!link_success) {
            glGetProgramInfoLog(program, info_log_length, NULL, info_log);
            LOG_ERROR(ERR_MIN, "Failed to link shader:\n%s", info_log);

            // Failed to create program so we free and leave.
            glDeleteProgram(program);
            return NULL;
        }
    }

    return program;
}

FILDEF Shader load_shader_from_file (const char* _file_name)
{
    // Build an absolute path to the file based on the executable location.
    std::string file_name(make_path_absolute(_file_name));

    char* source = read_entire_file(file_name.c_str());
    if (!source) {
        LOG_ERROR(ERR_MIN, "Failed to load shader '%s'!", file_name.c_str());
        return NULL;
    }
    defer { cstd_free(source); };

    Shader shader = load_shader_from_source(source);
    if (!shader) {
        LOG_ERROR(ERR_MIN, "Failed to build shader '%s'!", file_name.c_str());
    }

    return shader;
}

FILDEF void free_shader (Shader _program)
{
    glDeleteProgram(_program);
}

FILDEF void set_shader (Shader _shader)
{
    internal::current_shader = _shader;
    glUseProgram(internal::current_shader);
}
FILDEF Shader get_shader ()
{
    return internal::current_shader;
}

// @Speed: Could cache the locations of all a shader's uniforms in a std::map on load rather than retrieving them each time?

FILDEF void set_uniform_1f (const char* _name, GLfloat _a)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform1f(loc, _a);
}
FILDEF void set_uniform_2f (const char* _name, GLfloat _a, GLfloat _b)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform2f(loc, _a, _b);
}
FILDEF void set_uniform_3f (const char* _name, GLfloat _a, GLfloat _b, GLfloat _c)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform3f(loc, _a, _b, _c);
}
FILDEF void set_uniform_4f (const char* _name, GLfloat _a, GLfloat _b, GLfloat _c, GLfloat _d)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform4f(loc, _a, _b, _c, _d);
}

FILDEF void set_uniform_1i (const char* _name, GLint _a)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform1i(loc, _a);
}
FILDEF void set_uniform_2i (const char* _name, GLint _a, GLint _b)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform2i(loc, _a, _b);
}
FILDEF void set_uniform_3i (const char* _name, GLint _a, GLint _b, GLint _c)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform3i(loc, _a, _b, _c);
}
FILDEF void set_uniform_4i (const char* _name, GLint _a, GLint _b, GLint _c, GLint _d)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform4i(loc, _a, _b, _c, _d);
}

FILDEF void set_uniform_1fv (const char* _name, const GLfloat* _val)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform1fv(loc, 1, _val);
}
FILDEF void set_uniform_2fv (const char* _name, const GLfloat* _val)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform2fv(loc, 2, _val);
}
FILDEF void set_uniform_3fv (const char* _name, const GLfloat* _val)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform3fv(loc, 3, _val);
}
FILDEF void set_uniform_4fv (const char* _name, const GLfloat* _val)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform4fv(loc, 4, _val);
}

FILDEF void set_uniform_1iv (const char* _name, const GLint* _val)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform1iv(loc, 1, _val);
}
FILDEF void set_uniform_2iv (const char* _name, const GLint* _val)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform2iv(loc, 2, _val);
}
FILDEF void set_uniform_3iv (const char* _name, const GLint* _val)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform3iv(loc, 3, _val);
}
FILDEF void set_uniform_4iv (const char* _name, const GLint* _val)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform4iv(loc, 4, _val);
}

FILDEF void set_uniform_vec2 (const char* _name, Vec2 _vec)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform2f(loc, _vec.x, _vec.y);
}
FILDEF void set_uniform_vec3 (const char* _name, Vec3 _vec)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform3f(loc, _vec.x, _vec.y, _vec.z);
}
FILDEF void set_uniform_vec4 (const char* _name, Vec4 _vec)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniform4f(loc, _vec.x, _vec.y, _vec.z, _vec.w);
}

FILDEF void set_uniform_mat2 (const char* _name, Mat2 _mat)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(_mat));
}
FILDEF void set_uniform_mat3 (const char* _name, Mat3 _mat)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(_mat));
}
FILDEF void set_uniform_mat4 (const char* _name, Mat4 _mat)
{
    GLint loc = glGetUniformLocation(internal::current_shader, _name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(_mat));
}

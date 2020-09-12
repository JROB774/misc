typedef GLuint Shader;

STDDEF Shader load_shader_from_source (const char* _source);
FILDEF Shader load_shader_from_file   (const char* _file_name);
FILDEF void   free_shader             (Shader _program);

FILDEF void   set_shader (Shader _shader);
FILDEF Shader get_shader ();

// These are applied to the currently active/bound shader.

FILDEF void set_uniform_1f   (const char* _name, GLfloat _a);
FILDEF void set_uniform_2f   (const char* _name, GLfloat _a, GLfloat _b);
FILDEF void set_uniform_3f   (const char* _name, GLfloat _a, GLfloat _b, GLfloat _c);
FILDEF void set_uniform_4f   (const char* _name, GLfloat _a, GLfloat _b, GLfloat _c, GLfloat _d);

FILDEF void set_uniform_1i   (const char* _name, GLint _a);
FILDEF void set_uniform_2i   (const char* _name, GLint _a, GLint _b);
FILDEF void set_uniform_3i   (const char* _name, GLint _a, GLint _b, GLint _c);
FILDEF void set_uniform_4i   (const char* _name, GLint _a, GLint _b, GLint _c, GLint _d);

FILDEF void set_uniform_1fv  (const char* _name, const GLfloat* _val);
FILDEF void set_uniform_2fv  (const char* _name, const GLfloat* _val);
FILDEF void set_uniform_3fv  (const char* _name, const GLfloat* _val);
FILDEF void set_uniform_4fv  (const char* _name, const GLfloat* _val);

FILDEF void set_uniform_1iv  (const char* _name, const GLint* _val);
FILDEF void set_uniform_2iv  (const char* _name, const GLint* _val);
FILDEF void set_uniform_3iv  (const char* _name, const GLint* _val);
FILDEF void set_uniform_4iv  (const char* _name, const GLint* _val);

FILDEF void set_uniform_vec2 (const char* _name, Vec2 _vec);
FILDEF void set_uniform_vec3 (const char* _name, Vec3 _vec);
FILDEF void set_uniform_vec4 (const char* _name, Vec4 _vec);

FILDEF void set_uniform_mat2 (const char* _name, Mat2 _mat);
FILDEF void set_uniform_mat3 (const char* _name, Mat3 _mat);
FILDEF void set_uniform_mat4 (const char* _name, Mat4 _mat);

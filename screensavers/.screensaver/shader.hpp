#ifndef SHADER_HPP__ /*///////////////////////////////////////////////////////*/
#define SHADER_HPP__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

typedef GLuint Shader;

STDDEF bool load_shader_from_file (Shader& shader, const char* file_name);
STDDEF bool laod_shader_from_mem  (Shader& shader, const void* buf, size_t sz);

STDDEF void free_shader           (Shader& shader);

FILDEF void use_shader            (Shader& shader);

/* -------------------------------------------------------------------------- */

FILDEF void set_uniform_1f     (Shader& shader, const char* name, float a);
FILDEF void set_uniform_2f     (Shader& shader, const char* name, float a, float b);
FILDEF void set_uniform_3f     (Shader& shader, const char* name, float a, float b, float c);
FILDEF void set_uniform_4f     (Shader& shader, const char* name, float a, float b, float c, float d);
FILDEF void set_uniform_1i     (Shader& shader, const char* name, int a);
FILDEF void set_uniform_2i     (Shader& shader, const char* name, int a, int b);
FILDEF void set_uniform_3i     (Shader& shader, const char* name, int a, int b, int c);
FILDEF void set_uniform_4i     (Shader& shader, const char* name, int a, int b, int c, int d);
FILDEF void set_uniform_vec2   (Shader& shader, const char* name, vec2 vec);
FILDEF void set_uniform_vec3   (Shader& shader, const char* name, vec3 vec);
FILDEF void set_uniform_vec4   (Shader& shader, const char* name, vec4 vec);
FILDEF void set_uniform_1fv    (Shader& shader, const char* name, const float* val);
FILDEF void set_uniform_2fv    (Shader& shader, const char* name, const float* val);
FILDEF void set_uniform_3fv    (Shader& shader, const char* name, const float* val);
FILDEF void set_uniform_4fv    (Shader& shader, const char* name, const float* val);
FILDEF void set_uniform_1iv    (Shader& shader, const char* name, const int* val);
FILDEF void set_uniform_2iv    (Shader& shader, const char* name, const int* val);
FILDEF void set_uniform_3iv    (Shader& shader, const char* name, const int* val);
FILDEF void set_uniform_4iv    (Shader& shader, const char* name, const int* val);
FILDEF void set_uniform_mat2fv (Shader& shader, const char* name, int count, bool transpose, const float* val);
FILDEF void set_uniform_mat3fv (Shader& shader, const char* name, int count, bool transpose, const float* val);
FILDEF void set_uniform_mat4fv (Shader& shader, const char* name, int count, bool transpose, const float* val);

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* SHADER_HPP__ ///////////////////////////////////////////////////////*/

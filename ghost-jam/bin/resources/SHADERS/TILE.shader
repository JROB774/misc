// ADD ATTENUTATION TO MAKE THE LIGHT BECOME DISTANT: https://learnopengl.com/Lighting/Light-casters

#version 330

uniform sampler2D texture0;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 light_pos;
uniform float intensity;

// @Incomplete: Make this a uniform!
#if 0
const float dropoff = 20.0f;
#else
const float dropoff = 512.0f;
#endif

#if COMPILING_VERTEX_PROGRAM
    layout (location = 0) in vec3 in_pos;
    layout (location = 1) in vec2 in_texcoord;
    layout (location = 2) in vec4 in_color;
    layout (location = 3) in vec3 in_normal;

    out vec2 texcoord;
    out vec4 vert_color;
    out vec3 normal;
    out vec3 frag_pos;
    out float depth;

    void vert(){
        frag_pos = vec3(model * vec4(in_pos, 1.0f));
        normal = transpose(inverse(mat3(model))) * -in_normal; // We make it negative because the normals work with normal mapping but not without...
        gl_Position = projection * view * vec4(frag_pos, 1.0f);
        texcoord = in_texcoord;
        vert_color = in_color;
        depth = (dropoff - gl_Position.z) / dropoff;
    }
#elif COMPILING_FRAGMENT_PROGRAM
    in vec2 texcoord;
    in vec4 vert_color;
    in vec3 normal;
    in vec3 frag_pos;
    in float depth;

    out vec4 frag_color;

    void frag(){
        vec4 object_color = texture(texture0, texcoord) * vert_color;
        vec4 light_color = vec4(1.0f, 0.95f, 0.9f, 1.0f); // @Incomplete: Make this a uniform!

        object_color.rgb *= depth;

        // Ambient
        float ambient_strength = intensity;
        vec4 ambient = ambient_strength * light_color;

        // Diffuse
        vec3 norm = normalize(normal);
        vec3 light_dir = normalize(light_pos - frag_pos);
        float diff = max(dot(norm, light_dir), 0.0f);
        vec4 diffuse = diff * light_color;

        // Specular
        float specular_strength = 0.1f;
        vec3 view_pos = light_pos; // Our light pos currently is the player's camera pos.
        vec3 view_dir = normalize(view_pos - frag_pos);
        vec3 reflect_dir = reflect(-light_dir, norm);
        float shine = 32.0f;
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), shine);
        vec4 specular = specular_strength * spec * light_color;

        vec4 result = (ambient + diffuse + specular) * object_color;

        frag_color = vec4(result.rgb, 1.0f);
    }
#endif

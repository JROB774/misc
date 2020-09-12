struct Skybox
{
    VAO vao;
    VBO vbo;

    GLuint texture;
};

FILDEF void load_skybox (Skybox& _skybox, std::string name_);
FILDEF void free_skybox (Skybox& _skybox);

FILDEF void draw_skybox (const Skybox& _skybox);

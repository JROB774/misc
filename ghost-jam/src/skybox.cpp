const GLOBAL GLfloat SKYBOX_VERTS[]
{
    // Position
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

FILDEF void load_skybox (Skybox& _skybox, std::string name_)
{
    glGenVertexArrays(1, &_skybox.vao);
    glGenBuffers(1, &_skybox.vbo);

    glBindVertexArray(_skybox.vao);
    glBindBuffer(GL_ARRAY_BUFFER, _skybox.vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(SKYBOX_VERTS), &SKYBOX_VERTS, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glGenTextures(1, &_skybox.texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox.texture);

    std::vector<std::string> sides;
    sides.push_back(name_ + "R.png"); // Right
    sides.push_back(name_ + "L.png"); // Left
    sides.push_back(name_ + "U.png"); // Up
    sides.push_back(name_ + "D.png"); // Down
    sides.push_back(name_ + "F.png"); // Front
    sides.push_back(name_ + "B.png"); // Back

    int w,h,bpp;
    for (size_t i=0; i<sides.size(); ++i) {
        std::string file_name(sky_catalog.build_path(sides.at(i)));
        u8* data = stbi_load(file_name.c_str(), &w,&h,&bpp, 0);
        if (!data) {
            LOG_ERROR(ERR_MED, "Failed to load skybox texture \"%s\"!", file_name.c_str());
            return;
        }
        defer { stbi_image_free(data); };

        GLenum format;
        switch (bpp) {
        default:
        case(1): { format = GL_RED;  } break;
        case(2): { format = GL_RG;   } break;
        case(3): { format = GL_RGB;  } break;
        case(4): { format = GL_RGBA; } break;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+CAST(GLenum, i), 0, GL_RGB, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,   GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,   GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

FILDEF void free_skybox (Skybox& _skybox)
{
    glDeleteVertexArrays(1, &_skybox.vao);
    glDeleteBuffers(1, &_skybox.vbo);
    glDeleteTextures(1, &_skybox.texture);
}

FILDEF void draw_skybox (const Skybox& _skybox)
{
    // @Temporary
    PERSISTENT float time = 0.0f;
    time += dt;

    glDepthFunc(GL_LEQUAL);
    defer { glDepthFunc(GL_LESS); };

    set_shader(sky_catalog.GET_SHADER_RESOURCE("BASIC"));

    auto proj = get_projection();
    auto view = get_view();

    view = Mat4(Mat3(view)); // Remove translation from the view matrix.

    set_uniform_mat4("projection", proj);
    set_uniform_mat4("view", view);
    set_uniform_1f("time", time); // @Temporary

    glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox.texture);
    glBindVertexArray(_skybox.vao);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

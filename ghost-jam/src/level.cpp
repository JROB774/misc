namespace internal
{
    enum Old_Tile_Type
    {
        OLD_TILE_TYPE_WALL,
        OLD_TILE_TYPE_FLOOR,
        OLD_TILE_TYPE_START,
        OLD_TILE_TYPE_SPRITE,
    };

    struct Old_Tile_Info
    {
        std::string debug_name;
        Old_Tile_Type type;
        std::string texture;
        std::string sprite;
    };

    std::map<Old_Tile_ID, Old_Tile_Info> tile_info;
}

FILDEF bool init_tiles ()
{
    using namespace internal;

    // GonObject gon = GonObject::Load(make_path_absolute("resources/data/tiles.txt"));
    const GonObject& gon = load_gon_and_remove_comments(data_catalog.build_path("TILES_OLD.gon"));
    for (const auto& it: gon.children_map) {
        const GonObject& data = gon.children_array[it.second];

        Old_Tile_Info info;
        Old_Tile_ID id;

        std::string str_id = data["id"].String();
        id = CAST(Old_Tile_ID, std::stoul(str_id, 0, 16));
        info.debug_name = it.first;
        // load_texture_from_file(info.texture, std::string(make_path_absolute("resources/textures/") + data["texture"].String()).c_str());
        // load_texture_from_file(info.sprite, std::string(make_path_absolute("resources/textures/") + data["sprite"].String("empty.png")).c_str());
        info.texture = data["texture"].String();
        info.sprite = data["sprite"].String("empty");

        if(data["type"].String()      == "wall" )  {info.type = OLD_TILE_TYPE_WALL ;}
        else if(data["type"].String() == "floor")  {info.type = OLD_TILE_TYPE_FLOOR;}
        else if(data["type"].String() == "start")  {info.type = OLD_TILE_TYPE_START;}
        else if(data["type"].String() == "sprite") {info.type = OLD_TILE_TYPE_SPRITE;}

        tile_info.insert(std::pair<Old_Tile_ID, Old_Tile_Info>(id, info));
    }

    return true;
}

STDDEF void load_level (Level& _lvl, const char* _file_name)
{
    using namespace internal;

    std::string file_name(make_path_absolute(_file_name));
    u8* data = stbi_load(file_name.c_str(), &_lvl.width, &_lvl.height, NULL, 4);

    defer {
        stbi_image_free(data);
    };

    if(data == NULL){
        LOG_ERROR(ERR_MIN, "Failed to load level file \"%s\"!", file_name.c_str());
        return;
    }

    u32* new_data = (u32*)data;
    for(int i = 0; i < _lvl.width * _lvl.height; i++){
        Old_Tile_ID tile_value = SDL_Swap32(new_data[i]);
        _lvl.tiles.push_back(tile_value);
        if (tile_info.at(tile_value).type == OLD_TILE_TYPE_START) {
            set_player_pos(game.player, i % _lvl.width, i / _lvl.width);
            if (tile_value == 0xFF0000FF) {
                set_player_dir(game.player, PLAYER_DIR_NORTH);
            } else if (tile_value == 0x00FF00FF) {
                set_player_dir(game.player, PLAYER_DIR_EAST);
            } else if (tile_value == 0x0000FFFF) {
                set_player_dir(game.player, PLAYER_DIR_SOUTH);
            } else if (tile_value == 0xFFFF00FF) {
                set_player_dir(game.player, PLAYER_DIR_EAST);
            }
        }
    }
}

STDDEF void draw_level (Level& _lvl)
{
    using namespace internal;

    // PERSISTENT float timer = 0.0f;
    // timer += dt;

    set_shader(shader_catalog.GET_SHADER_RESOURCE("TILE"));

    // float light_intensity = random_float(0.5f, 0.55f);
    float light_intensity = 0.25f;
    Vec3 light_pos(game.player.camera.pos);

    set_uniform_mat4("projection", get_projection());
    set_uniform_mat4("view", get_view());
    set_uniform_vec3("light_pos", light_pos);
    set_uniform_1i  ("texture0", GL_TEXTURE0);
    set_uniform_1f  ("intensity", light_intensity);

    // if (timer >= 0.05f) {
    //     set_float_uniform(textured, "intensity", light_intensity);
    //     timer = 0.0f;
    // }

    // set_sampler_uniform(textured, "normal0", GL_TEXTURE1);

    const float step = 1.0f;

    float offx = 0.0f;
    float offz = 0.0f;

    for (size_t i=0; i<_lvl.tiles.size(); ++i) {
        const auto& id = _lvl.tiles.at(i);
        const auto& info = tile_info.at(id);

        if (i != 0 && (i % _lvl.width == 0)) {
            offx  = 0.0f;
            offz += step;
        }

        float offy = 0.0f;
        if (info.type != OLD_TILE_TYPE_WALL) {
            offy = -1.0f;
        }

        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, get_texture_resource("normal." + info.texture).handle);

        set_texture(texture_catalog.GET_TEXTURE_RESOURCE(info.texture));

        if (info.type == OLD_TILE_TYPE_WALL) {
            glm::mat4 model_matrix(1);
            model_matrix = glm::translate(model_matrix, glm::vec3(offx, offy, offz));
            draw_mesh(cube_mesh, model_matrix);
        } else {
            glm::mat4 model_matrix(1);
            model_matrix = glm::translate(model_matrix, glm::vec3(offx, offy, offz));
            draw_mesh(plane_mesh, model_matrix);
        }

        offx += step;
    }

    ///////////////////////////////////

    draw_skybox(skybox);

    ///////////////////////////////////

    // @Incomplete: Need to draw from back-to-front to avoid clipping issues with other sprites + use single-point particle thing for drawing.
    // @Incomplete: Need to draw from back-to-front to avoid clipping issues with other sprites + use single-point particle thing for drawing.
    // @Incomplete: Need to draw from back-to-front to avoid clipping issues with other sprites + use single-point particle thing for drawing.

    set_shader(shader_catalog.GET_SHADER_RESOURCE("SPRITE"));

    set_uniform_mat4("projection", get_projection());
    set_uniform_mat4("view", get_view());
    set_uniform_vec3("light_pos", light_pos);
    set_uniform_1i  ("texture0", GL_TEXTURE0);
    set_uniform_1f  ("intensity", light_intensity);

    // set_vec3_uniform(billboard, "light_pos", light_pos);
    // set_float_uniform(billboard, "intensity", light_intensity);

    offx = 0.0f;
    offz = 0.0f;

    for (size_t i=0; i<_lvl.tiles.size(); ++i) {
        auto& id = _lvl.tiles.at(i);
        auto& info = tile_info.at(id);

        if (i != 0 && (i % _lvl.width == 0)) {
            offx  = 0.0f;
            offz += step;
        }

        if (info.type == OLD_TILE_TYPE_SPRITE) {
            glm::mat4 model_matrix(1);
            model_matrix = glm::translate(model_matrix, glm::vec3(offx, 0.0f, offz-0.5f));
            set_texture(sprite_catalog.GET_TEXTURE_RESOURCE(info.sprite));
            draw_mesh(sprite_mesh, model_matrix);
        }

        offx += step;
    }

    /*
    set_shader(textured_shader);
    glBindVertexArray(vao);

    internal__set_mat4_uniform(textured_shader, "projection", projection_matrix);
    internal__set_mat4_uniform(textured_shader, "view", view_matrix);
    internal__set_sampler_uniform(textured_shader, "texture0", GL_TEXTURE0);

    const float step = 1.0f;

    float offx = 0.0f;
    float offz = 0.0f;

    for (size_t i=0; i<_lvl.tiles.size(); ++i) {
        const auto& id = _lvl.tiles.at(i);
        const auto& info = tile_info.at(id);

        if (i != 0 && (i % _lvl.width == 0)) {
            offx  = 0.0f;
            offz += step;
        }

        float offy = 0.0f;
        if (info.type == OLD_TILE_TYPE_FLOOR || info.type == OLD_TILE_TYPE_START || info.type == OLD_TILE_TYPE_SPRITE) {
            offy = -1.0f;
        }

        model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, glm::vec3(offx, offy, offz));
        internal__set_mat4_uniform(textured_shader, "model", model_matrix);

        glBindTexture(GL_TEXTURE_2D, info.texture.handle);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        offx += step;
    }

    ///////////////////////////////////

    // @Incomplete: Need to draw from back-to-front to avoid clipping issues with other sprites!
    // @Incomplete: Need to draw from back-to-front to avoid clipping issues with other sprites!
    // @Incomplete: Need to draw from back-to-front to avoid clipping issues with other sprites!

    set_shader(billboard_shader);
    glBindVertexArray(plane_vao);

    internal__set_mat4_uniform(billboard_shader, "projection", projection_matrix);
    internal__set_mat4_uniform(billboard_shader, "view", view_matrix);
    internal__set_sampler_uniform(billboard_shader, "texture0", GL_TEXTURE0);

    offx = 0.0f;
    offz = 0.0f;

    for (size_t i=0; i<_lvl.tiles.size(); ++i) {
        const auto& id = _lvl.tiles.at(i);
        const auto& info = tile_info.at(id);

        if (i != 0 && (i % _lvl.width == 0)) {
            offx  = 0.0f;
            offz += step;
        }

        if (info.type == OLD_TILE_TYPE_SPRITE) {
            model_matrix = glm::mat4(1);
            model_matrix = glm::translate(model_matrix, glm::vec3(offx, 0.0f, offz));
            internal__set_mat4_uniform(billboard_shader, "model", model_matrix);

            glBindTexture(GL_TEXTURE_2D, info.sprite.handle);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        offx += step;
    }
    */
}


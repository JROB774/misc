namespace internal
{
    GLOBAL const std::map<std::string, Tile_Type> TILE_TYPE_MAP
    {
    { "WALL", TILE_TYPE_WALL },
    { "FLAT", TILE_TYPE_FLAT },
    { "CEIL", TILE_TYPE_CEIL }
    };

    GLOBAL const std::map<std::string, Thing_Type> THING_TYPE_MAP
    {
    { "CONTROL", THING_TYPE_CONTROL },
    { "ITEM",    THING_TYPE_ITEM    },
    { "FRIEND",  THING_TYPE_FRIEND  },
    { "MONSTER", THING_TYPE_MONSTER },
    { "PROP",    THING_TYPE_PROP    }
    };

    GLOBAL std::map<Tile_ID, Tile_Info> tile_info_map;
    GLOBAL std::map<Thing_ID, Thing_Info> thing_info_map;

    FILDEF void read_c_str (std::string& _str, FILE* _file)
    {
        _str.clear();

        char c = 0;
        do {
            fread(&c, sizeof(char), 1, _file);
            if (c) { _str.push_back(c); }
        }
        while (c);
    }

    FILDEF void load_area_v0 (Area& _area, FILE* _file)
    {
        // HEADER
        fread(&_area.header.width,            sizeof(_area.header.width),       1, _file);
        fread(&_area.header.height,           sizeof(_area.header.height),      1, _file);
        fread(&_area.header.thing_count,      sizeof(_area.header.thing_count), 1, _file);
        read_c_str(_area.header.name,                                              _file);
        read_c_str(_area.header.effect,                                            _file);
        read_c_str(_area.header.skybox,                                            _file);
        read_c_str(_area.header.skybox_effect,                                     _file);

        // TILES
        u32 lw = _area.header.width;
        u32 lh = _area.header.height;

        for (int i=0; i<Area::TILE_LAYER_TOTAL; ++i) {
            _area.tiles.at(i).clear();
            _area.tiles.at(i).resize(lw*lh, Tile());
        }

        for (auto& layer: _area.tiles) {
            for (auto& tile: layer) {
                fread(&tile.id,    sizeof(Tile_ID),    1, _file);
                fread(&tile.flags, sizeof(Tile_Flags), 1, _file);

                u8 r, g, b;

                fread(&r, sizeof(u8), 1, _file);
                fread(&g, sizeof(u8), 1, _file);
                fread(&b, sizeof(u8), 1, _file);

                tile.tint.r = CAST(float, r) / 255.0f;
                tile.tint.g = CAST(float, g) / 255.0f;
                tile.tint.b = CAST(float, b) / 255.0f;
            }
        }

        // THINGS
        _area.things.clear();

        for (u16 i=0; i<_area.header.thing_count; ++i) {
            Thing thing;

            fread(&thing.id,    sizeof(Thing_ID),         1, _file);
            fread(&thing.flags, sizeof(Thing_Flags),      1, _file);
            fread(&thing.dir,   sizeof(Thing::Direction), 1, _file);

            u32 x, y, z;

            fread(&x, sizeof(u32), 1, _file);
            fread(&y, sizeof(u32), 1, _file);
            fread(&z, sizeof(u32), 1, _file);

            thing.pos.x = (CAST(float, x) / CAST(float, SUB_TILE_SIZE));
            thing.pos.y = (CAST(float, y) / CAST(float, SUB_TILE_SIZE));
            thing.pos.z = (CAST(float, z) / CAST(float, SUB_TILE_SIZE));

            u8 r, g, b;

            fread(&r, sizeof(u8), 1, _file);
            fread(&g, sizeof(u8), 1, _file);
            fread(&b, sizeof(u8), 1, _file);

            thing.tint.r = CAST(float, r) / 255.0f;
            thing.tint.g = CAST(float, g) / 255.0f;
            thing.tint.b = CAST(float, b) / 255.0f;

            _area.things.push_back(thing);
        }
    }

    FILDEF void save_area_v0 (Area& _area, FILE* _file)
    {
        _area.header.thing_count = CAST(u16, _area.things.size());

        // HEADER
        fwrite(&_area.header.version,               sizeof(_area.header.version),     1,                                     _file);
        fwrite(&_area.header.width,                 sizeof(_area.header.width),       1,                                     _file);
        fwrite(&_area.header.height,                sizeof(_area.header.height),      1,                                     _file);
        fwrite(&_area.header.thing_count,           sizeof(_area.header.thing_count), 1,                                     _file);
        fwrite( _area.header.name.c_str(),          sizeof(char),                     _area.header.name         .length()+1, _file);
        fwrite( _area.header.effect.c_str(),        sizeof(char),                     _area.header.effect       .length()+1, _file);
        fwrite( _area.header.skybox.c_str(),        sizeof(char),                     _area.header.skybox       .length()+1, _file);
        fwrite( _area.header.skybox_effect.c_str(), sizeof(char),                     _area.header.skybox_effect.length()+1, _file);

        // TILES
        for (const auto& layer: _area.tiles) {
            for (const auto& tile: layer) {
                fwrite(&tile.id,    sizeof(Tile_ID),    1, _file);
                fwrite(&tile.flags, sizeof(Tile_Flags), 1, _file);

                u8 r = CAST(u8, tile.tint.r * 255.0f);
                u8 g = CAST(u8, tile.tint.g * 255.0f);
                u8 b = CAST(u8, tile.tint.b * 255.0f);

                fwrite(&r, sizeof(u8), 1, _file);
                fwrite(&g, sizeof(u8), 1, _file);
                fwrite(&b, sizeof(u8), 1, _file);
            }
        }

        // THINGS
        for (const auto& thing: _area.things) {
            fwrite(&thing.id,    sizeof(Thing_ID),         1, _file);
            fwrite(&thing.flags, sizeof(Thing_Flags),      1, _file);
            fwrite(&thing.dir,   sizeof(Thing::Direction), 1, _file);

            u32 x = CAST(u32, (thing.pos.x * SUB_TILE_SIZE));
            u32 y = CAST(u32, (thing.pos.y * SUB_TILE_SIZE));
            u32 z = CAST(u32, (thing.pos.z * SUB_TILE_SIZE));

            fwrite(&x, sizeof(u32), 1, _file);
            fwrite(&y, sizeof(u32), 1, _file);
            fwrite(&z, sizeof(u32), 1, _file);

            u8 r = CAST(u8, thing.tint.r * 255.0f);
            u8 g = CAST(u8, thing.tint.g * 255.0f);
            u8 b = CAST(u8, thing.tint.b * 255.0f);

            fwrite(&r, sizeof(u8), 1, _file);
            fwrite(&g, sizeof(u8), 1, _file);
            fwrite(&b, sizeof(u8), 1, _file);
        }
    }
}

FILDEF void load_tile_data ()
{
    LOG_DEBUG("Loading Tiles");

    const auto& gon = load_gon_and_remove_comments(data_catalog.build_path("TILES.gon"));
    for (const auto it: gon.children_map) {
        const auto& data = gon.children_array[it.second];

        Tile_ID id = CAST(Tile_ID, data["id"].Number());
        Tile_Info info;

        info.name = it.first;
        info.type = internal::TILE_TYPE_MAP.at(data["type"].String());
        info.texture = data["texture"].String();

        internal::tile_info_map.insert(std::pair<Tile_ID, Tile_Info>(id, info));

        #if 0
        printf("Tile %d:\n", id);
        printf("  Name: %s\n", info.name.c_str());
        printf("  Type: %d\n", info.type);
        printf("  Texture: %s\n", info.texture.c_str());
        #endif
    }
}

FILDEF void load_thing_data ()
{
    LOG_DEBUG("Loading Things");

    const auto& gon = load_gon_and_remove_comments(data_catalog.build_path("THINGS.gon"));
    for (const auto it: gon.children_map) {
        const auto& data = gon.children_array[it.second];

        Thing_ID id = CAST(Thing_ID, data["id"].Number());
        Thing_Info info;

        info.name = it.first;
        info.type = internal::THING_TYPE_MAP.at(data["type"].String());
        info.sprite = data["sprite"].String();
        info.visible = data["visible"].Bool(true);

        internal::thing_info_map.insert(std::pair<Thing_ID, Thing_Info>(id, info));

        #if 0
        printf("Thing %d:\n", id);
        printf("  Name: %s\n", info.name.c_str());
        printf("  Type: %d\n", info.type);
        printf("  Sprite: %s\n", info.sprite.c_str());
        printf("  Visible: %s\n", info.visible?"true":"false");
        #endif
    }
}

FILDEF const Tile_Info& get_tile_info (Tile_ID _id)
{
    return internal::tile_info_map.at(_id);
}
FILDEF const Thing_Info& get_thing_info (Thing_ID _id)
{
    return internal::thing_info_map.at(_id);
}

STDDEF void load_area (Area& _area, const char* _file_name)
{
    FILE* file = fopen(_file_name, "rb");
    if (!file) {
        LOG_ERROR(ERR_MED, "Failed to read level \"%s\"!", _file_name);
        return;
    }
    defer { fclose(file); };

    fread(&_area.header.version, sizeof(_area.header.version), 1, file);
    switch (_area.header.version) {
    case (0): { internal::load_area_v0(_area, file); } break;
    }
}

STDDEF void save_area (Area& _area, const char* _file_name)
{
    FILE* file = fopen(_file_name, "wb");
    if (!file) {
        LOG_ERROR(ERR_MED, "Failed to save level \"%s\"!", _file_name);
        return;
    }
    defer { fclose(file); };

    // We always save from the file version to the current highest version.
    // This way older files can still be used, modified, and then upgraded.
    switch (_area.header.version) {
    case (0): { internal::save_area_v0(_area, file); } break;
    }
}

STDDEF void free_area (Area& _area)
{
    // Nothing
}

FILDEF Area* get_current_area ()
{
    // @Incomplete: ...
    return NULL;
}

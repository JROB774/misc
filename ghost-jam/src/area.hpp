#pragma once

GLOBAL constexpr u32 CURRENT_AREA_VERSION = 0;

// How many sub-locations within a tile that a THING can be placed on.
GLOBAL constexpr u32 SUB_TILE_SIZE = 4;

typedef u16 Tile_ID;
typedef u16 Thing_ID;

typedef u32 Tile_Flags;
typedef u32 Thing_Flags;

enum Tile_Type
{
    // Correspond to Area::Tile_Layer values.
    TILE_TYPE_FLAT,
    TILE_TYPE_WALL,
    TILE_TYPE_CEIL,
};

enum Thing_Type
{
    THING_TYPE_CONTROL,
    THING_TYPE_ITEM,
    THING_TYPE_FRIEND,
    THING_TYPE_MONSTER,
    THING_TYPE_PROP,
};

struct Tile_Info
{
    std::string name;
    Tile_Type type;
    std::string texture;
};

struct Thing_Info
{
    std::string name;
    Thing_Type type;
    std::string sprite;
    bool visible;
};

struct Tile
{
    Tile_ID id = 0;
    Tile_Flags flags = 0;

    Vec3 tint = Vec3(1,1,1);
};

struct Thing
{
    enum Direction: u8
    {
        DIR_N  = 0x01,
        DIR_E  = 0x02,
        DIR_S  = 0x04,
        DIR_W  = 0x08,

        DIR_NE = DIR_N|DIR_E,
        DIR_SE = DIR_S|DIR_E,
        DIR_SW = DIR_S|DIR_W,
        DIR_NW = DIR_N|DIR_W,
    };

    Thing_ID id = 0;
    Thing_Flags flags = 0;

    Direction dir = DIR_N;

    Vec3 pos;
    Vec3 tint = Vec3(1,1,1);
};

struct Area
{
    enum Tile_Layer
    {
        TILE_LAYER_FLAT,
        TILE_LAYER_WALL,
        TILE_LAYER_CEIL,

        TILE_LAYER_TOTAL
    };

    struct Header
    {
        u32         version;
        u16         width;
        u16         height;
        u16         thing_count;
        std::string name;
        std::string effect;
        std::string skybox;
        std::string skybox_effect;
    };

    // Used by the built-in editor.
    std::string file_name;

    Header header;

    std::array<std::vector<Tile>, TILE_LAYER_TOTAL> tiles;
    std::vector<Thing> things;

    Skybox skybox;
};

FILDEF void load_tile_data ();
FILDEF void load_thing_data ();

FILDEF const Tile_Info& get_tile_info (Tile_ID _id);
FILDEF const Thing_Info& get_thing_info (Thing_ID _id);

STDDEF void load_area (Area& _area, const char* _file_name);
STDDEF void save_area (Area& _area, const char* _file_name);

STDDEF void free_area (Area& _area);

FILDEF Area* get_current_area ();

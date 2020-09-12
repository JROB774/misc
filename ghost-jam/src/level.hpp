typedef u32 Old_Tile_ID;

struct Level
{
    int width;
    int height;

    std::vector<Old_Tile_ID> tiles;
};

FILDEF bool init_tiles ();

STDDEF void load_level (Level& _lvl, const char* _file_name);
STDDEF void draw_level (Level& _lvl);

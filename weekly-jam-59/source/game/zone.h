/*******************************************************************************
 * ZONE
 * Systems for loading the different zones of the game.
 *
*******************************************************************************/

#ifndef __TCE_ZONE_H__ /*/////////////////////////////////////////////////////*/
#define __TCE_ZONE_H__

namespace TCE
{

GLOBAL constexpr const char* ZONE_TILE_EXTENSION   = ".png";
GLOBAL constexpr const char* ZONE_ENTITY_EXTENSION = ".entity";

GLOBAL constexpr const char* ZONE_PATH = "data/zones/";

GLOBAL constexpr int TILE_WIDTH  = 20;
GLOBAL constexpr int TILE_HEIGHT = 20;

enum TileType : u32
{
	//                    R G B A
	TILE_TYPE_NONE    = 0x00000000, // None tiles are transparent.
	TILE_TYPE_FLOOR   = 0x0000FF00, // Floor tiles are blue channel.
	TILE_TYPE_WALL    = 0xFF000000, // Wall tiles are red channel.
	TILE_TYPE_CEILING = 0x00FF0000  // Ceiling tiles are green channel.
};
enum TileMask : u8
{
	TILE_MASK_UP      = 0x01,       // A tile is located up.
	TILE_MASK_RIGHT   = 0x02,       // A tile is located right.
	TILE_MASK_DOWN    = 0x04,       // A tile is located down.
	TILE_MASK_LEFT    = 0x08,       // A tile is located left

	TILE_MASK_ALL     = 0x0F
};

struct Tile
{
	u32 id;
	u8  mask_flags;
};

struct Zone
{
	static constexpr int BYTES_PER_PIXEL = 4;

	int width = 0;
	int height = 0;

	unsigned int seed = 0;

	Array<Tile> tile_data;
};

// We store a copy of the currently loaded zone C++ side because it is much easier
// and faster to do pathfinding over here and then to pass the result back to Lua.
GLOBAL Zone current_zone;

INLDEF void LoadZoneFromFile (const char* _filename);

} // TCE

#endif /* __TCE_ZONE_H__ /////////////////////////////////////////////////////*/

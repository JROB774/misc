/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void LoadZoneFromFile (const char* _filename)
{
	// Clear the old data from the zone to replace with new data.
	current_zone.width = 0;
	current_zone.height = 0;
	current_zone.seed = 0;
	current_zone.tile_data.Clear();

	// Combine the zone path with the filename and extension.
	String filename = ZONE_PATH;
	filename.AddCString(_filename);
	filename.AddCString(ZONE_TILE_EXTENSION);

	// Create a random seed based on the zone's title, that is used for random tiles.
	current_zone.seed = 0;
	for (const char* c=_filename; *c; ++c) {
		current_zone.seed += *c;
	}
	srand(current_zone.seed);

	// Load in the zone data from an image so we can convert it into actual tiles.
	int width = 0, height = 0;
	u8* pixel_data = stbi_load(filename.c_string, &width, &height, NULL, Zone::BYTES_PER_PIXEL);
	if (!pixel_data) {
		TCE_ERROR_LOG(ERROR_LEVEL_MED, ERROR_TYPE_STD, "Failed to load zone file '%s'!", _filename);
	} else {
		int zone_size = width * height;
		// Preallocate the tile data to make the operation faster.
		current_zone.width = width, current_zone.height = height;
		current_zone.tile_data.Resize(zone_size);

		// Go through and determine the id, mask, etc. of each of the zone's tiles."
		int* tile_data = CAST(int*, pixel_data);
		for (int y=0; y<height; ++y) {
			for (int x=0; x<width; ++x) {
				Tile& tile = current_zone.tile_data[y * width + x];
				u32 tile_id = SDL_SwapBE32(tile_data[y * width + x]); // To RGBA.

				// If the tile had no alpha then it is a NONE type tile and should be set to all zeroes.
				if (!(tile_id & 0x000000FF)) { tile.id = TILE_TYPE_NONE; continue; }
				else { tile.id = tile_id & ~0x000000FF; }

				// Depending on the type of tile we are determines what adjacent tiles we want to check for.
				u32 types_to_check_for = 0;
				if      (tile.id & TILE_TYPE_FLOOR)   { types_to_check_for = TILE_TYPE_WALL;    } // Floors check for walls.
				else if (tile.id & TILE_TYPE_WALL)    { types_to_check_for = TILE_TYPE_WALL;    } // Walls check for walls.
				else if (tile.id & TILE_TYPE_CEILING) { types_to_check_for = TILE_TYPE_CEILING; } // Ceilings check for ceilings.

				// Go through and set the appropriate flags based on adjacent tiles.
				tile.mask_flags = 0;

				u32 up     = (y == 0)          ? tile.id : (SDL_SwapBE32(tile_data[(y-1) * width +  x   ]) & ~0x000000FF); // If top of level or tile is above.
				u32 right  = (x >= (width-1))  ? tile.id : (SDL_SwapBE32(tile_data[ y    * width + (x+1)]) & ~0x000000FF); // If right of level or tile is to the right.
				u32 bottom = (y >= (height-1)) ? tile.id : (SDL_SwapBE32(tile_data[(y+1) * width +  x   ]) & ~0x000000FF); // If bottom of level or tile is below.
				u32 left   = (x == 0)          ? tile.id : (SDL_SwapBE32(tile_data[ y    * width + (x-1)]) & ~0x000000FF); // If left of level or tile is to the left.

				bool tile_is_floor = tile.id & TILE_TYPE_FLOOR;

				if ((up     & types_to_check_for) && (tile_is_floor || tile.id == up))     { tile.mask_flags |= TILE_MASK_UP;    }
				if ((right  & types_to_check_for) && (tile_is_floor || tile.id == right))  { tile.mask_flags |= TILE_MASK_RIGHT; }
				if ((bottom & types_to_check_for) && (tile_is_floor || tile.id == bottom)) { tile.mask_flags |= TILE_MASK_DOWN;  }
				if ((left   & types_to_check_for) && (tile_is_floor || tile.id == left))   { tile.mask_flags |= TILE_MASK_LEFT;  }
			}
		}
	}

	// Make sure to clean-up after we're done.
	stbi_image_free(pixel_data);
	pixel_data = NULL;
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/

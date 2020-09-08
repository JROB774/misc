namespace LVL
{
	// this needs to be in correct order for z-ordering
	enum LayerType
	{
		LAYER_TAG,
		LAYER_OVERLAY,
		LAYER_ACTIVE, // combo of basic and enemy
		LAYER_BACK1,
		LAYER_BACK2,
		LAYER_TOTAL
	};

	enum CategoryType
	{
		CATEGORY_BASIC,
		CATEGORY_TAG,
		CATEGORY_OVERLAY,
		CATEGORY_ENEMY,
		CATEGORY_BACK, // combo of back 1 and 2
		CATEGORY_TOTAL
	};

	constexpr std::uint32_t OFFSET[] =
	{
		  0, // BASIC
		 41, // TAG
		147, // OVERLAY
		157, // ENEMY
		279  // BACK
	};

	typedef std::array<std::vector<std::uint32_t>, LAYER_TOTAL> MapArray;

	namespace
	{
		enum MagicNummber
		{
			BASIC_MAGICNUM   = 0,
			TAG_MAGICNUM     = 20000,
			OVERLAY_MAGICNUM = 30000,
			ENEMY_MAGICNUM   = 40000,
			BACK_MAGICNUM    = 50000
		};

		// the order in which the game stores layer data (why is everything always changing?!?)
		constexpr std::size_t IO_ORDER[] = {
			LAYER_BACK1,
			LAYER_ACTIVE,
			LAYER_TAG,
			LAYER_OVERLAY,
			LAYER_BACK2
		};

		std::uint32_t GetSaveID (std::uint32_t _id)
		{			
			// determine what category each tile is in, remove the offset and add the magic number
			     if (_id >= OFFSET[CATEGORY_BACK])    { return (_id - OFFSET[CATEGORY_BACK]) + BACK_MAGICNUM; }
			else if (_id >= OFFSET[CATEGORY_ENEMY])   { return (_id - OFFSET[CATEGORY_ENEMY]) + ENEMY_MAGICNUM; }
			else if (_id >= OFFSET[CATEGORY_OVERLAY]) { return (_id - OFFSET[CATEGORY_OVERLAY]) + OVERLAY_MAGICNUM; }
			else if (_id >= OFFSET[CATEGORY_TAG])     { return (_id - OFFSET[CATEGORY_TAG]) + TAG_MAGICNUM; }
			// if none of the above then it is a basic tile
			return (_id - OFFSET[CATEGORY_BASIC] + BASIC_MAGICNUM);
		}

		std::uint32_t GetLoadID (std::uint32_t _id)
		{
			// determine what category each tile is in, remove the magic number and add the offset
			     if (_id >= BACK_MAGICNUM)    { return (_id - BACK_MAGICNUM) + OFFSET[CATEGORY_BACK]; }
			else if (_id >= ENEMY_MAGICNUM)   { return (_id - ENEMY_MAGICNUM) + OFFSET[CATEGORY_ENEMY]; }
			else if (_id >= OVERLAY_MAGICNUM) { return (_id - OVERLAY_MAGICNUM) + OFFSET[CATEGORY_OVERLAY]; }
			else if (_id >= TAG_MAGICNUM)     { return (_id - TAG_MAGICNUM) + OFFSET[CATEGORY_TAG]; }
			// if none of the above then it is a basci tile
			return (_id - BASIC_MAGICNUM + OFFSET[CATEGORY_BASIC]);
		}
	}

	void Save (std::string _filename, const MapArray& _tiledata, std::uint32_t _lvlwidth, std::uint32_t _lvlheight)
	{
		std::ofstream output(_filename, std::ios::trunc|std::ios::binary);
		if (!output.is_open()) {
			ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to save '" + _filename + "'!");
			return;
		}

		// cache big endian versions of the header values
		std::uint32_t version = SDL_SwapBE32(1);
		std::uint32_t width = SDL_SwapBE32(_lvlwidth);
		std::uint32_t height = SDL_SwapBE32(_lvlheight);
		std::uint32_t layers = SDL_SwapBE32(LAYER_TOTAL);
		// write the header info (version, width, height, numlayers)
		output.write((char*)&version, sizeof(version));
		output.write((char*)&width, sizeof(width));
		output.write((char*)&height, sizeof(height));
		output.write((char*)&layers, sizeof(layers));

		// write the raw layer data for the level
		for (std::size_t i = 0; i < LAYER_TOTAL; ++i) {
			auto& layer = _tiledata.at(IO_ORDER[i]);
			for (auto& tile: layer) {
				std::uint32_t tileid = GetSaveID(tile);
				tileid = SDL_SwapBE32(tileid);
				output.write((char*)&tileid, sizeof(tileid));
			}
		}

		// close the file after use
		output.close();
	}

	void Load (std::string _filename, MapArray& _tiledata, std::uint32_t& _lvlwidth, std::uint32_t& _lvlheight)
	{
		std::ifstream input(_filename, std::ios::binary);
		if (!input.is_open()) {
			ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to open '" + _filename + "'!");
			return;
		}

		// read in the header data, we only need the width and height though
		std::uint32_t dummy;
		input.read((char*)&dummy, sizeof(dummy)); // version
		input.read((char*)&_lvlwidth, sizeof(_lvlwidth));
		input.read((char*)&_lvlheight, sizeof(_lvlheight));
		input.read((char*)&dummy, sizeof(dummy)); // layers

		// handle swapping the width and height values if needed
		_lvlwidth = SDL_SwapBE32(_lvlwidth);
		_lvlheight = SDL_SwapBE32(_lvlheight);

		// dynamically allocate the tiledata to fit the level size
		for (auto& layer: _tiledata) { layer.resize(_lvlwidth * _lvlheight); }

		// read the raw layer data for the level
		for (std::size_t i = 0; i < LAYER_TOTAL; ++i) {
			auto& layer = _tiledata.at(IO_ORDER[i]);
			for (auto& tile: layer) {
				input.read((char*)&tile, sizeof(tile));
				tile = SDL_SwapBE32(tile);
				tile = GetLoadID(tile);
			}
		}

		// close the file after use
		input.close();
	}
}
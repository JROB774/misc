local TCE = require("data.scripts.tce.tce")

Map = {}

Map.ROOM_WIDTH  = TCE.GetViewportWidth() / TILE_WIDTH
Map.ROOM_HEIGHT = TCE.GetViewportHeight() / TILE_HEIGHT

function Map.loadZone (filename)
	-- Load the parsed zone data from C++.
	local zone = TCE.LoadZoneFromFile(filename)
	-- We always want the random floor tiles to be the same.
	math.randomseed(zone.seed)

	-- Convert the raw tile data into something usable by the game.
	for x = 0, (zone.width - 1), 1 do
		for y = 0, (zone.height - 1), 1 do
			if zone.tile_data[1 + y *zone.width + x] ~= -1 then
				-- Various values defining a given tile.
				local tileID = zone.tile_data[1 + y * zone.width + x].id
				local maskFlags = zone.tile_data[1 + y * zone.width + x].mask_flags
				local textureOffset = blocks[tileID].textureOffset
				local tileType = blocks[tileID].tileType
				local zIndex = blocks[tileID].zIndex

				-- If the tile is a floor then determine its random texture clip.
				local renderOffset = 0
				if tileType == TILE_TYPE.FLOOR then
					-- The flag determing whether a wall is above is 0x01 which means all shadows will be odd and all other floors even.
					if maskFlags % 2 == 0 then renderOffset = TCE.LowWeightedRandom(0, 7) * TILE_CLIP_WIDTH
					else renderOffset = TCE.LowWeightedRandom(8, 15) * TILE_CLIP_WIDTH end
				end

				Tile.new(
					{ x = x, y = y },
					textureOffset, tileType, maskFlags, renderOffset, zIndex)
			end
		end
	end

	-- Load zone is polite and reseeds random after it is done loading.
	math.randomseed(TCE.RandomSeed())

	-- Set the current zone to the newly loaded zone.
	currentZone = filename
end

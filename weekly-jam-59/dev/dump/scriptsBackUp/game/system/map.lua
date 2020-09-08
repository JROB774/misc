local TCE = require("data.scripts.tce.tce")

Map = {}

function Map.loadZone (filename)
	-- Load the parsed zone data from C++.
	local zone = TCE.LoadZoneFromFile(filename)

	-- Convert the raw tile data into something usable by the game.
	for ix = 0, (zone.width - 1), 1 do
		for iy = 0, (zone.height - 1), 1 do
			if zone.tile_data[1 + iy *zone.width + ix] ~= -1 then
				local tileID = zone.tile_data[1 + iy * zone.width + ix].id
				local renderOffset = zone.tile_data[1 + iy * zone.width + ix].render_offset

				Tile.new(ix * TILE_WIDTH, iy * TILE_HEIGHT, blocks[tileID].texture, blocks[tileID].tileType, 0)
			end
		end
	end
end

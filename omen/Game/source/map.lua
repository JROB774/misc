--[[
Map script contains the namespace/table for the current map. This
namespace contains multiple members storing information regarding
the map. This data includes the name, size and raw data of the map.

Maps are laaded in from text files (.map) using a simple format and
parsed into tile-data that can be handled accordingly by the game.
--]]

-- includes functionality from other scripts
require("source.utility")
require("source.constant")
require("source.raycast")

-- the global map namespace is created
Map = {}
Map.__index = Map

-- base/starting information is set
Map.caption = "NULL"
Map.width = 0
Map.height = 0

--[[
This function takes in a filename (this file must be located in the
../assets/maps/ path to be accessed) and loads the file for reading. The
info from the file is parsed and converted into map data that can be used
and understood by the game/application.

Only one map can be loaded at a time, so the previously loaded map will
be nullified as the new one is loaded in.
--]]

function Map.load (_filename)
	-- attempts to open the map file, if it cant then an error is thrown
	local file, error = io.open(system.pathForFile("assets/maps/" .. _filename))
	if not file then error("Map read error: " .. error, 1) end

	-- ensures that if there was a previous map it is nullified
	for iy = 1, Map.height do
		for ix = 1, Map.width do
			Map[iy][ix] = nil
		end
	end

	-- reads the first three lines of the file to get the caption, width, and height
	Map.caption = file:read("*l")
	Map.width = tonumber(file:read("*l"))
	Map.height = tonumber(file:read("*l"))

	-- iterates through all the following lines of the file
	for iy = 1, Map.height do
		local row = file:read("*l")
		Map[iy] = {}
		-- goes character-by-character to get the tile data for the map
		for ix = 1, Map.width do
			-- # characters are solid walls, anything else is an empty tile spot
			if row:sub(ix, ix) == '#' then Map[iy][ix] = Constant.TILE_SOLID
			else Map[iy][ix] = Constant.TILE_EMPTY end
		end
	end

	-- the wall texture information is then loaded in for the map to use and render as slices
	Map.wall = {}
	Map.wall.texture = graphics.newImageSheet(
		"assets/images/wall.png",
		{ width = 1, height = Constant.TILE_HEIGHT, numFrames = Constant.TILE_WIDTH, sheetContentWidth = Constant.TILE_WIDTH, sheetContentHeight = Constant.TILE_HEIGHT }
	)
	-- each texture is split into single-pixel strips which will be scaled vertically to fit the screen slices
	Map.wall.slices = {
		{ name =  "1", start =  1, count = 1, time = 0 }, { name =  "2", start =  2, count = 1, time = 0 },
		{ name =  "3", start =  3, count = 1, time = 0 }, { name =  "4", start =  4, count = 1, time = 0 },
		{ name =  "5", start =  5, count = 1, time = 0 }, { name =  "6", start =  6, count = 1, time = 0 },
		{ name =  "7", start =  7, count = 1, time = 0 }, { name =  "8", start =  8, count = 1, time = 0 },
		{ name =  "9", start =  9, count = 1, time = 0 }, { name = "10", start = 10, count = 1, time = 0 },
		{ name = "11", start = 11, count = 1, time = 0 }, { name = "12", start = 12, count = 1, time = 0 },
		{ name = "13", start = 13, count = 1, time = 0 }, { name = "14", start = 14, count = 1, time = 0 },
		{ name = "15", start = 15, count = 1, time = 0 }, { name = "16", start = 16, count = 1, time = 0 },
		{ name = "17", start = 17, count = 1, time = 0 }, { name = "18", start = 18, count = 1, time = 0 },
		{ name = "19", start = 19, count = 1, time = 0 }, { name = "20", start = 20, count = 1, time = 0 },
		{ name = "21", start = 21, count = 1, time = 0 }, { name = "22", start = 22, count = 1, time = 0 },
		{ name = "23", start = 23, count = 1, time = 0 }, { name = "24", start = 24, count = 1, time = 0 },
		{ name = "25", start = 25, count = 1, time = 0 }, { name = "26", start = 26, count = 1, time = 0 },
		{ name = "27", start = 27, count = 1, time = 0 }, { name = "28", start = 28, count = 1, time = 0 },
		{ name = "29", start = 29, count = 1, time = 0 }, { name = "30", start = 30, count = 1, time = 0 },
		{ name = "31", start = 31, count = 1, time = 0 }, { name = "32", start = 32, count = 1, time = 0 }
	}

	-- every screen slice is then saved, these will be filled in with appropriate wall slices, every frame
	Map.slices = {}
	for i = 1, Constant.PROJPLANE_WIDTH do
		Map.slices[i] = display.newSprite(Map.wall.texture, Map.wall.slices)
		Map.slices[i].x = i - 1
		Map.slices[i].y = Constant.PROJPLANE_HEIGHT / 2
	end
end

--[[
This handler is called every frame the map is active. It takes raycast
information provided by the player namespace/table and uses it to accurately
project the preloaded wall texture across the individual screen slices.
This is how the walls of the map are rendered and it is what gives the 3D
effect to the first-person world.
--]]

function Map.enterFrame (_event)
	-- if there is no ray information to use then do not attempt to render (the info is needed)
	if Raycast.rays ~= nil and Raycast.offsets ~= nil and Raycast.dirs ~= nil then
		-- go through and render each vertical slice of the screen
		for i = 1, Constant.PROJPLANE_WIDTH do
			-- use the length of the ray (distance of the wall from the player) to determine the slice's projected height and brigthness
			local height = math.ceil(Constant.TILE_HEIGHT / Raycast.rays[i] * Constant.PROJPLANE_DISTANCE)
			local shadow = 1.0 - (Raycast.rays[i] / Constant.TILE_DISTANCE_SHADOW)
			-- if the wall is horizontal then add an extra bit of shadow for more depth
			if Raycast.dirs[i] == "horz" then shadow = shadow - Constant.TILE_SIDE_SHADOW end

			-- if the ray is out of bounds (too long) then do not draw any wall slice
			if Raycast.rays[i] == Constant.RAY_OUT_OF_BOUNDS then Map.slices[i].isVisible = false
			else
				-- otherwise draw a wall slice using the info just calculated
				Map.slices[i].isVisible = true
				Map.slices[i]:setSequence(tostring(Raycast.offsets[i] + 1))
				Map.slices[i].height = height
				Map.slices[i]:setFillColor(shadow, shadow, shadow)
			end
		end
	end
end
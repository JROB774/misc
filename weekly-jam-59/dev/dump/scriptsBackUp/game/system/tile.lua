local TCE = require("data.scripts.tce.tce")

Tile = setmetatable({}, {__index = Entity})
Tile.__index = Slime
function Tile.new(x, y, tex, tileType, renderoffset)
	local isSolid = (tileType == TILE_TYPE.WALL_TOP or tileType == TILE_TYPE.WALL_SIDE)
	local colliderHeight = tileType == TILE_TYPE.WALL_SIDE and TILE_HEIGHT / 3 or TILE_HEIGHT

	local self = setmetatable(
		Entity.new(x, y, TILE_WIDTH, colliderHeight, 2, 0, 0, 0, isSolid, true, true, tex, "tile"),
		Tile)

	self.renderoffset = renderoffset

	return self
end

function Tile.render()

end

TILE_ID = {}

TILE_ID.WALL_GRASS_LEFT                  =  421075455
TILE_ID.WALL_GRASS_MIDDLE                =  255
TILE_ID.WALL_GRASS_RIGHT                 =  640034559

TILE_ID.FLAT_GRASS_TOP_TOPLEFT           =  1717987071
TILE_ID.FLAT_GRASS_TOP_TOPMIDDLE         =  2139062271
TILE_ID.FLAT_GRASS_TOP_TOPRIGHT          =  1920103167
TILE_ID.FLAT_GRASS_TOP_LEFT              = -1936945921
TILE_ID.FLAT_GRASS_TOP_MIDDLE            =  1077952767
TILE_ID.FLAT_GRASS_TOP_RIGHT             = -1717986817
TILE_ID.FLAT_GRASS_TOP_BOTTOMLEFT        =  1280068863
TILE_ID.FLAT_GRASS_TOP_BOTTOMMIDDLE      = -1515870721
TILE_ID.FLAT_GRASS_TOP_BOTTOMRIGHT       =  1499027967

TILE_ID.FLAT_DIRT_BOTTOM_PLAIN           = -1
TILE_ID.FLAT_DIRT_BOTTOM_SHADOW_LEFT     = -858993409
TILE_ID.FLAT_DIRT_BOTTOM_SHADOW_MIDDLE   = -437918209
TILE_ID.FLAT_DIRT_BOTTOM_SHADOW_RIGHT    = -656877313
TILE_ID.FLAT_DIRT_BOTTOM_SHADOW_DETAIL_1 = -1077952513
TILE_ID.FLAT_DIRT_BOTTOM_SHADOW_DETAIL_2 = -1296911617

--[[
TILE_IDS.RED    = -16777216
TILE_IDS.YELLOW = -65536
TILE_IDS.GREEN  = 16711935
TILE_IDS.CYAN   = 16776960
TILE_IDS.BLUE   = 65280
TILE_IDS.PINK   = -16771936
--]]

TILE_TYPE = {}

TILE_TYPE.WALL_TOP  = 1
TILE_TYPE.WALL_SIDE = 2
TILE_TYPE.FLOOR     = 3

blocks =
{
	--[-1] = function() end;
	[TILE_ID.WALL_GRASS_LEFT]                  = { texture = TCE.LoadTextureFromFile("walls/wall_grass_left.png"),                tileType = TILE_TYPE.WALL_SIDE };
	[TILE_ID.WALL_GRASS_MIDDLE]                = { texture = TCE.LoadTextureFromFile("walls/wall_grass_middle.png"),              tileType = TILE_TYPE.WALL_SIDE };
	[TILE_ID.WALL_GRASS_RIGHT]                 = { texture = TCE.LoadTextureFromFile("walls/wall_grass_right.png"),               tileType = TILE_TYPE.WALL_SIDE };
	[TILE_ID.FLAT_GRASS_TOP_TOPLEFT]           = { texture = TCE.LoadTextureFromFile("flats/flat_grass_top_topleft.png"),         tileType = TILE_TYPE.WALL_TOP  };
	[TILE_ID.FLAT_GRASS_TOP_TOPMIDDLE]         = { texture = TCE.LoadTextureFromFile("flats/flat_grass_top_topmiddle.png"),       tileType = TILE_TYPE.WALL_TOP  };
	[TILE_ID.FLAT_GRASS_TOP_TOPRIGHT]          = { texture = TCE.LoadTextureFromFile("flats/flat_grass_top_topright.png"),        tileType = TILE_TYPE.WALL_TOP  };
	[TILE_ID.FLAT_GRASS_TOP_LEFT]              = { texture = TCE.LoadTextureFromFile("flats/flat_grass_top_left.png"),            tileType = TILE_TYPE.WALL_TOP  };
	[TILE_ID.FLAT_GRASS_TOP_MIDDLE]            = { texture = TCE.LoadTextureFromFile("flats/flat_grass_top_middle.png"),          tileType = TILE_TYPE.WALL_TOP  };
	[TILE_ID.FLAT_GRASS_TOP_RIGHT]             = { texture = TCE.LoadTextureFromFile("flats/flat_grass_top_right.png"),           tileType = TILE_TYPE.WALL_TOP  };
	[TILE_ID.FLAT_GRASS_TOP_BOTTOMLEFT]        = { texture = TCE.LoadTextureFromFile("flats/flat_grass_top_bottomleft.png"),      tileType = TILE_TYPE.WALL_TOP  };
	[TILE_ID.FLAT_GRASS_TOP_BOTTOMMIDDLE]      = { texture = TCE.LoadTextureFromFile("flats/flat_grass_top_bottommiddle.png"),    tileType = TILE_TYPE.WALL_TOP  };
	[TILE_ID.FLAT_GRASS_TOP_BOTTOMRIGHT]       = { texture = TCE.LoadTextureFromFile("flats/flat_grass_top_bottomright.png"),     tileType = TILE_TYPE.WALL_TOP  };

	[TILE_ID.FLAT_DIRT_BOTTOM_PLAIN]           = { texture = TCE.LoadTextureFromFile("flats/flat_dirt_bottom_plain.png"),         tileType = TILE_TYPE.FLOOR     };
	[TILE_ID.FLAT_DIRT_BOTTOM_SHADOW_LEFT]     = { texture = TCE.LoadTextureFromFile("flats/flat_dirt_bottom_shadow_left.png"),   tileType = TILE_TYPE.FLOOR     };
	[TILE_ID.FLAT_DIRT_BOTTOM_SHADOW_MIDDLE]   = { texture = TCE.LoadTextureFromFile("flats/flat_dirt_bottom_shadow_middle.png"), tileType = TILE_TYPE.FLOOR     };
	[TILE_ID.FLAT_DIRT_BOTTOM_SHADOW_RIGHT]    = { texture = TCE.LoadTextureFromFile("flats/flat_dirt_bottom_shadow_right.png"),  tileType = TILE_TYPE.FLOOR     };
	[TILE_ID.FLAT_DIRT_BOTTOM_SHADOW_DETAIL_1] = { texture = TCE.LoadTextureFromFile("flats/flat_dirt_bottom_detail_1.png"),      tileType = TILE_TYPE.FLOOR     };
	[TILE_ID.FLAT_DIRT_BOTTOM_SHADOW_DETAIL_2] = { texture = TCE.LoadTextureFromFile("flats/flat_dirt_bottom_detail_2.png"),      tileType = TILE_TYPE.FLOOR     };
}

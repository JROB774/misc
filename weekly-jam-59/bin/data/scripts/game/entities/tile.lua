local TCE = require("data.scripts.tce.tce")

TILE_CLIP_WIDTH = 40
TILE_CLIP_HEIGHT = 40

TILE_TYPE =
{
	NONE     = 0;
	FLOOR    = 1;
	WALL     = 2;
	CEILING  = 3;
}

TILE_ID =
{
	NONE               = tonumber("0x00000000");
	CEILING_TEST       = tonumber("0x001F0000");
	CEILING_GRASS      = tonumber("0x00FF0000");
	CEILING_DIRT       = tonumber("0x00CF0000");
	CEILING_DARKDIRT   = tonumber("0x00AF0000");
	CEILING_STONE      = tonumber("0x007F0000");
	CEILING_STONEGRASS = tonumber("0x003F0000");
	WALL_TEST          = tonumber("0x1F000000");
	WALL_GRASS         = tonumber("0xFF000000");
	WALL_DIRT          = tonumber("0xCF000000");
	WALL_DARKDIRT      = tonumber("0xAF000000");
	WALL_STONE         = tonumber("0x7F000000");
	WALL_STONEGRASS    = tonumber("0x3F000000");
	FLOOR_TEST         = tonumber("0x00001F00");
	FLOOR_DIRT         = tonumber("0x0000FF00");
	FLOOR_DARKGRASS    = tonumber("0x00009000");
	FLOOR_STONE        = tonumber("0x0000CF00");
	FLOOR_DARKDIRT     = tonumber("0x00007F00");
	FLOOR_GRASS        = tonumber("0x00003F00");
}

TILE_MASK =
{
	TILE_MASK_UP    =  1;
	TILE_MASK_RIGHT =  2;
	TILE_MASK_DOWN  =  4;
	TILE_MASK_LEFT  =  8;

	TILE_MASK_ALL   = 15;
}

--- TILE CLASS
Tile = setmetatable({}, {__index = Entity})
Tile.__index = Tile
function Tile.new(pos, textureOffset, tileType, maskFlags, renderOffset, zIndex)
	-- If the tile is a floor tile then we do not want it to be solid.
	local isSolid = (tileType == TILE_TYPE.CEILING or tileType == TILE_TYPE.WALL)
	-- If the tile is a wall or a top-level ceiling then it has a smaller collider.
	local renderYOffset = 0
	local colliderHeight = TILE_HEIGHT
	if tileType == TILE_TYPE.WALL then
		colliderHeight = TILE_HEIGHT / 3
	elseif tileType == TILE_TYPE.CEILING and not TCE.TestBitFlag(maskFlags, TILE_MASK.TILE_MASK_UP) then
		renderYOffset = TILE_HEIGHT / 2
		colliderHeight = TILE_HEIGHT / 2
	end

	local self = setmetatable(
		Entity.new(
			{ x = pos.x, y = pos.y + (renderYOffset / TILE_HEIGHT) }, TILE_WIDTH, colliderHeight, 0,
			{health = 10,
			  attack = 0,
			  speed = {x = 0, y = 0},
			  velocity = {x = 0, y = 0},
			  lastVelocity = {x = 0, y = 0}},
			{solid = isSolid,
			  renderOn = true,
			  updateOn = false,
			  tag = "tile",
			  tileType = tileType,
			  maskFlags = maskFlags,
			  renderOffset = renderOffset},
			tex["tileset"]),
		Tile)

	self.textureOffset = textureOffset
	self.renderYOffset = renderYOffset

	self.zIndex = zIndex

	self:uniqueIdAssign()

	tileList[self.id] = self
	reserveTileList[self.id] = self
	--renderList[self.id] = self
	return self
end

function  Tile:update(dt, player)
	if self:onScreenCheck() == false then
		self.flags.renderOn = true
		tileList[self.id] = reserveTileList[self.id]
	else
		self.flags.renderOn = false
		tileList[self.id] = nil
	end
end

function Tile:render()
	-- Do not bother rendering a tile if it is a non-existent tile.
	if self.flags.tileType == TILE_TYPE.NONE then return end

	local renderX = (self.pos.x + (TILE_WIDTH / 2)) - (TILE_CLIP_WIDTH / 2)
	local renderY = ((self.pos.y + (TILE_HEIGHT / 2)) - (TILE_CLIP_HEIGHT / 2)) - self.renderYOffset

	-- Render the correct grpahic from a tile's tileset.
	local clip = { x = self.flags.maskFlags * TILE_CLIP_WIDTH, y = self.textureOffset * TILE_CLIP_HEIGHT, w = TILE_CLIP_WIDTH, h = TILE_CLIP_HEIGHT }
	if self.flags.tileType == TILE_TYPE.FLOOR then clip.x = self.flags.renderOffset end

	TCE.RenderClippedTexture(tex["tileset"], clip, { x = renderX, y = renderY })

	-- Render some debug information regarding tile bit-masking.
	if Debug.ShowTileInfo then
		local textX = self.pos.x + 2
		local textY = self.pos.y + 2

		TCE.RenderRectFilled(self.pos.x, self.pos.y - self.renderYOffset, TILE_WIDTH, TILE_HEIGHT, "0x0000003F")
		TCE.RenderShadowedText(fnt["debug"], tostring(self.flags.maskFlags), "0xFFFFFFFF", "0x000000FF", 1, 1, { x = textX, y = textY - self.renderYOffset })
	end
end

--- TILE SPECIFIC VALUES

blocks =
{
	[TILE_ID.NONE]               = { textureOffset =  0, tileType = TILE_TYPE.NONE,    zIndex = -1                      };
	[TILE_ID.CEILING_TEST]       = { textureOffset = 15, tileType = TILE_TYPE.CEILING, zIndex = Z_INDEX.CEILING_BASIC   };
	[TILE_ID.CEILING_GRASS]      = { textureOffset =  0, tileType = TILE_TYPE.CEILING, zIndex = Z_INDEX.CEILING_BASIC   };
	[TILE_ID.CEILING_DIRT]       = { textureOffset =  4, tileType = TILE_TYPE.CEILING, zIndex = Z_INDEX.CEILING_BASIC   };
	[TILE_ID.CEILING_DARKDIRT]   = { textureOffset =  7, tileType = TILE_TYPE.CEILING, zIndex = Z_INDEX.CEILING_BASIC   };
	[TILE_ID.CEILING_STONE]      = { textureOffset = 10, tileType = TILE_TYPE.CEILING, zIndex = Z_INDEX.CEILING_BASIC   };
	[TILE_ID.CEILING_STONEGRASS] = { textureOffset = 13, tileType = TILE_TYPE.CEILING, zIndex = Z_INDEX.CEILING_BASIC   };
	[TILE_ID.WALL_TEST]          = { textureOffset = 16, tileType = TILE_TYPE.WALL,    zIndex = Z_INDEX.WALL_BASIC      };
	[TILE_ID.WALL_GRASS]         = { textureOffset =  1, tileType = TILE_TYPE.WALL,    zIndex = Z_INDEX.WALL_BASIC      };
	[TILE_ID.WALL_DIRT]          = { textureOffset =  5, tileType = TILE_TYPE.WALL,    zIndex = Z_INDEX.WALL_BASIC      };
	[TILE_ID.WALL_DARKDIRT]      = { textureOffset =  8, tileType = TILE_TYPE.WALL,    zIndex = Z_INDEX.WALL_BASIC      };
	[TILE_ID.WALL_STONE]         = { textureOffset = 11, tileType = TILE_TYPE.WALL,    zIndex = Z_INDEX.WALL_BASIC      };
	[TILE_ID.WALL_STONEGRASS]    = { textureOffset = 14, tileType = TILE_TYPE.WALL,    zIndex = Z_INDEX.WALL_BASIC      };
	[TILE_ID.FLOOR_TEST]         = { textureOffset = 17, tileType = TILE_TYPE.FLOOR,   zIndex = Z_INDEX.FLOOR_BASIC     };
	[TILE_ID.FLOOR_DIRT]         = { textureOffset =  6, tileType = TILE_TYPE.FLOOR,   zIndex = Z_INDEX.FLOOR_DIRT      };
	[TILE_ID.FLOOR_STONE]        = { textureOffset = 12, tileType = TILE_TYPE.FLOOR,   zIndex = Z_INDEX.FLOOR_STONE     };
	[TILE_ID.FLOOR_DARKDIRT]     = { textureOffset =  9, tileType = TILE_TYPE.FLOOR,   zIndex = Z_INDEX.FLOOR_DARKDIRT  };
	[TILE_ID.FLOOR_GRASS]        = { textureOffset =  2, tileType = TILE_TYPE.FLOOR,   zIndex = Z_INDEX.FLOOR_GRASS     };
	[TILE_ID.FLOOR_DARKGRASS]    = { textureOffset =  3, tileType = TILE_TYPE.FLOOR,   zIndex = Z_INDEX.FLOOR_DARKGRASS };
}

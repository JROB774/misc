
-- The higher the z-index the higher up the object will be drawn. If two entities share
-- a z-index value then their draw-order will be determined by their y coordinate. E.G.
-- The draw order of player and walls will be determined this way as they both use 500.

Z_INDEX =
{
	FLOOR_BASIC     =     0;
	FLOOR_STONE     =     4;
	FLOOR_DIRT      =     5;
	FLOOR_DARKDIRT  =    10;
	FLOOR_GRASS     =    50;
	FLOOR_DARKGRASS =    60;
	WALL_BASIC      =   500;
	PLAYER          =   500;
	ENTITY_BASIC    =   500;
	CEILING_BASIC   =  1000;
	ENTITY_SCRIMBAT =  5000;
	BULLET          = 10000;
}

--[[
Raycast script contains the namespace that has all the ray data.
This data can be used and modified by multiple scripts to allow
for raycasting-based rendering.
--]]

-- the global raycast namespace
Raycast = {}
Raycast.__index = Raycast

-- stores ray information for the game
Raycast.rays = {}
-- stores the offset of the ray within the given tile
Raycast.offsets = {}
-- stores whether the ray is horz or vert
Raycast.dirs = {}
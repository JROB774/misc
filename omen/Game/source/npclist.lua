--[[
NPC List script contains a table that stores all the currently
active NPCs within the game world. This is useful so that multiple
different files can access the NPC list information.
--]]

-- the global npclist namespace is created
Npclist = {}
Npclist.__index = Npclist

-- stores the NPCs that are currently in play
Npclist.npcs = {}
-- stores how many NPCs are currently in play
Npclist.count = 0
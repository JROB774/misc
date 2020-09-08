--[[
NPC List script contains a table that stores all the currently
active NPCs within the game world. This is useful so that multiple
different files can access the NPC list information.
--]]

-- the global npclist namespace
Npclist = {}
Npclist.__index = Npclist

-- stores the npcs that are currently in play
Npclist.npcs = {}
Npclist.count = 0
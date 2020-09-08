local TCE = require("data.scripts.tce.tce")

-- Spawns item of type

SpawnItem = setmetatable({}, {__index = Entity})
SpawnItem.__index = SpawnItem
function SpawnItem.new(pos, itemType)
	local self = setmetatable(
		Entity.new(
			pos, 20, 20, 0,
			{health = 1,
				maxHealth = 1,
				attack = 0,
				velocity = {x = 30, y = 30},
				lastVelocity = {x = 0, y = 0},
				stunDelay = {timer = 0, max = 2}},
			{solid = false,
				renderOn = true,
				updateOn = true,
				tag = "pickUp",
				state = "",
				item = itemType},
			tex[itemType]),
		SpawnItem)

	self:uniqueIdAssign()

    entityList[self.id] = self
	renderList[self.id] = self
	return self
end

function SpawnItem:update(dt, player)
	if (self:onScreenCheck()) then
    	self.flags.renderOn = false
		self.flags.updateOn = false
	end
	if checkColSpecific(player, self) then
		player:updateItems(self.flags.item)
		print(itemType)
		self.flags.renderOn = false
		self.flags.updateOn = false
	end
end

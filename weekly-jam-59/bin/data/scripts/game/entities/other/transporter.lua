local TCE = require("data.scripts.tce.tce")

Transporter = setmetatable({}, {__index = Entity})
Transporter.__index = Transporter
function Transporter.new(pos, teleZone, telePos)
	local self = setmetatable(
		Entity.new(
			pos,
			20, 20, 0,
			{health = 1,
		 		maxHealth = 1,
				attack = 0, 
				velocity = {x = 0, y = 0},
				lastVelocity = {x = 0, y = 0},
				stunDelay = {timer = 0, max = 0},
				teleZone = teleZone,
				telePos = {x = telePos.x * TILE_WIDTH, y = telePos.y * TILE_HEIGHT}},
			{solid = true, 
				renderOn = true, 
				updateOn = true, 
				tag = "transport",
				state = ""},
			tex["teleporter"]),
		Transporter)

	self:uniqueIdAssign()

    entityList[self.id] = self
	renderList[self.id] = self
	return self
end

function Transporter:update(dt, player)
	print(self.stats.teleZone)

	if checkColSpecific(player, self) then
		currentZone = self.stats.teleZone
		player.pos = self.stats.telePos
		loadEnemies()
		
	end

	if (self:onScreenCheck()) then 
		self.flags.renderOn = false
		self.flags.updateOn = false
		self.flags.solid = false
	end
end

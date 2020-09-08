local TCE = require("data.scripts.tce.tce")

-- All directional rush Enemy that deals damage on contact.
Slime = setmetatable({}, {__index = Entity})
Slime.__index = Slime
function Slime.new(pos, angle)
	local self = setmetatable(
		Entity.new(
			pos,
			20, 20, 0,
			{health = 1,
				maxHealth = 1, 
				attack = 0, 
				velocity = {x = 10, y = 10},
				lastVelocity = {x = 0, y = 0},
				stunDelay = {timer = 0, max = 2}},
			{solid = true, 
				renderOn = true, 
				updateOn = true, 
				tag = "enemy",
				state = ""},
			tex["slime"]),
		Slime)

	self:uniqueIdAssign()
	entityList[self.id] = self
	renderList[self.id] = self
	return self
end

function Slime:update(dt, player)
	-- @TODO Maybe extract this bit?
	if self.flags.state ~= "stunned" then
		self:standardEnemyColCheck()
		self:stunTimer(dt)
	
		self:movement(dt, player)
	end
	if (self:onScreenCheck()) then 
		self.flags.renderOn = false
		self.flags.updateOn = false
		self.flags.solid = false
	end
end

-- Movement style is rush.
function Slime:movement(dt, player)
	local dir = TCE.LookAtVector(player.pos, self.pos)
    self.pos.x = self.pos.x + dir.x * dt * self.stats.velocity.x
    self.pos.y = self.pos.y + dir.y * dt * self.stats.velocity.y

	self.stats.lastVelocity.x = self.stats.velocity.x * dir.x
	self.stats.lastVelocity.y = self.stats.velocity.y * dir.y
end
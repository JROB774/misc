local TCE = require("data.scripts.tce.tce")

-- All directional rush Enemy that deals damage on contact.
Slime = setmetable({}, {__index = Entity})
Slime.__index = Slime
function Slime.new(pos, angle)
	local self = setmetable(
		Entity.new(
			pos, 20, 20, 0,
			{health = 10, 
			  attack = 0, 
			  velocity = {x = 10, y = 10}
			  lastVelocity = {x = 0, y = 0}}),
			{solid = true, 
			  renderOn = true, 
			  updateOn = true, 
			  tag = "enemy",
			  state = ""},
			tex["slime"]),
		Slime)

	table.insert(entityList, self)
	table.insert(renderList, self)

	return self
end

function Slime:update(dt, player)
	-- @TODO Maybe extract this bit?
	self:standardEnemyColCheck()

	self:movement(dt, player)

	if (self:onScreenCheck) then 
		self.flags.renderOn = false
		self.flags.updateOn = false
	end
end

-- Movement style is rush.
function Slime:movement(dt, player)
	local dir = TCE.lookAtVector(player.pos, self.pos)
    self.pos.x = self.pos.x + dir.x * dt * self.stats.velocity.x
    self.pos.y = self.pos.y + dir.y * dt * self.stats.velocity.y

	self.stats.lastVelocity.x = self.stats.velocity.x * dir.x
	self.stats.lastVelocity.y = self.stats.velocity.y * dir.y
end
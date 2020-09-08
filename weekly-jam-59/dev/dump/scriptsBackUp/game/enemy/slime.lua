local TCE = require("data.scripts.tce.tce")

-- All directional rush Enemy that deals damage on contact.

Slime = setmetatable({}, {__index = Entity})
Slime.__index = Slime
function Slime.new(x, y, angle)
	local self = setmetatable(
		Entity.new(x, y, TILE_WIDTH, TILE_HEIGHT, 2, 1, 35, angle, true, true, true, tex["slime"], "enemy"),
		Slime)

	return self
end

function Slime:update(dt, player)
	self:checkColTiles()

	if self:checkColSpecific(player) then
		player:dealDmg(self.attack)
	end

	self:movement(dt, player)
end

-- Movement style is rush.
function Slime:movement(dt, player)
	local dir = self:lookAtVector(player.pos, self.pos)
    self.pos.x = self.pos.x + dir.x * dt * self.speed.x
    self.pos.y = self.pos.y + dir.y * dt * self.speed.y

	self.lastVelocity.x = self.speed.x * dir.x
	self.lastVelocity.y = self.speed.y * dir.y
end

local TCE = require("data.scripts.tce.tce")

-- Cycloid wave (jumps) rush enemy. Damages on contact.

MadRabbit = setmetatable({}, {__index = Entity})
MadRabbit.__index = MadRabbit
function MadRabbit.new(x, y, angle)
	local self = setmetatable(
		Entity.new(x, y, TILE_WIDTH, TILE_HEIGHT, 2, 1, 35, angle, true, true, true, tex["madRabbit"], "enemy"),
		MadRabbit)

	self.theta = 0
	self.waveAmp = 0.75

	return self
end

function MadRabbit:update(dt, player)
	self:checkColTiles()

	if self:checkColSpecific(player) then
		player:dealDmg(self.attack)
	end

	self:movement(dt, player)
end

function MadRabbit:movement(dt, player)
	local dir = self:lookAtVector(player.pos, self.pos)

	-- Rotation
	local dirAngle = math.tan(dir.y, dir.x) - TCE.MathConstants.M_PI / 2
	self.angle = dirAngle 
	self.theta = self.theta + dt --* self.waveAmp
	--print(self.theta)
	--print("Theta: ", theta)
	local wave = {}
	wave.x = 0.25 * self.theta
	wave.y = self.waveAmp * math.sin(self.theta/5)

	dir.x = (wave.x * math.cos(dirAngle)) - (wave.y * math.sin(dirAngle))
	dir.y = (wave.x * math.sin(dirAngle)) + (wave.y * math.cos(dirAngle))

	--print("dir: ", dir.x, dir.y)

    self.pos.x = self.pos.x + dir.x * dt * self.speed.x
    self.pos.y = self.pos.y + dir.y * dt * self.speed.y

	self.lastVelocity.x = self.speed.x * dir.x
	self.lastVelocity.y = self.speed.y * dir.y
end

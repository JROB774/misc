local TCE = require("data.scripts.tce.tce")

-- Stays a distance away from player and stabs. High health points and enemies
-- hide behind.

Defender = setmetatable({}, {__index = Entity})
Defender.__index = Defender
function Defender.new(x, y, angle)
	local self = setmetatable(
		Entity.new(x, y, TILE_WIDTH, TILE_HEIGHT, 20, 1, 10, angle, true, true, true, tex["defender"], "enemy"),
		Defender)

	self.radius = 60
	return self
end

function Defender:update(dt, player)
	self:checkColTiles()

	self:movement(dt, player)

	if self:checkColSpecific(player) then
		player:dealDmg(self.attack)
	end

	-- @TODO got to stabilises the defending phase. Use rounding.
	if self.state ~= "rushing" and
		(self.lastAttack ~= nil and
		self.lastAttack.timeDeath == true) or
		self.lastAttack == nil then
		
		-- @TODO Work out start position depending on starting angle. Probs best to do it in bullet
		self.lastAttack = EnemyBullet.new(self.pos.x - 27, self.pos.y, 50, 10, 6, 0.25, 10, player.pos, tex["greatSwordSlash"], player)
	end

	--self.collisions = {}
	for key in pairs(entityList) do
		entityList[key].defenderPresent = false
        if entityList[key].solid == true and
            TCE.BoxAndBoxCollision(
            {x = self.pos.x, y = self.pos.y, w = self.width, h = self.height},
            {x = entityList[key].pos.x, y = entityList[key].pos.y, w = entityList[key].width, h = entityList[key].height}) then
        	entityList[key].defenderPresent = true
        end
    end
end

function Defender:movement(dt, player)
	local dir = self:lookAtVector(player.pos, self.pos)
	
	-- @TODO Killing on collision with wall me thinks. I think it just turn off render
	if TCE.RoundNum(self.radius, 2) < TCE.RoundNum(math.abs(dir.length), 2) then
		self.state = "rushing"

		self.pos.x = self.pos.x + dir.x * dt * self.speed.x
    	self.pos.y = self.pos.y + dir.y * dt * self.speed.y

		self.lastVelocity.x = self.speed.x * dir.x
		self.lastVelocity.y = self.speed.y * dir.y
	elseif TCE.RoundNum(self.radius, 2) > TCE.RoundNum(math.abs(dir.length), 2)then
		self.pos.x = self.pos.x + (-dir.x) * dt * self.speed.x
    	self.pos.y = self.pos.y + (-dir.y) * dt * self.speed.y

		self.lastVelocity.x = self.speed.x * -dir.x
		self.lastVelocity.y = self.speed.y * -dir.y

		self.state = "running"
	else
		self.state = "defend"
	end
end
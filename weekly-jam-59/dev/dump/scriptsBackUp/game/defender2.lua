local TCE = require("data.scripts.tce.tce")

-- Stays a distance away from player and stabs. High health points and enemies
-- hide behind.

Defender = setmetatable({}, {__index = Entity})
Defender.__index = Defender
function Defender.new(pos)
	local self = setmetable(
		Entity.new(
			pos, 20, 20, 0,
			{health = 30, 
			  attack = 6, 
			  velocity = {x = 10, y = 10},
			  lastVelocity = {x = 0, y = 0},
			  radius = 60}),
			{solid = true, 
			  renderOn = true, 
			  updateOn = true, 
			  tag = "enemy",
			  state = "rushing"},
			tex["defender"]),
		Defender)

	table.insert(entityList, self)
	table.insert(renderList, self)

	return self
end

function Defender:update(dt, player)
	self:standardEnemyColCheck()

	-- AI
	self:stateCheck()
	self:movement()
	self:attackAI()
	-- @INCOMPLETE ? Add a way for units to hide behind this unit? 

	if (self:onScreenCheck) then 
		self.flags.renderOn = false
		self.flags.updateOn = false
	end
end

function Defender:stateCheck()
	local dir = TCE.lookAtVector(player.pos, self.pos)
	local offset = 10

	if self.radius < maths.abs(dir.length + offset) then
		self.state = "rushing"
	elseif self.radius > maths.abs(dir.length + offset) then
		self.state = "runningAway"
	else 
		self.state = "attack"
	end
end

function Defender:movement(dt, player)
	local dir = TCE.lookAtVector(player.pos, self.pos)
	if self.state == "rushing" then
		self.pos.x = self.pos.x + dir.x * dt * self.stats.velocity.x
    	self.pos.y = self.pos.y + dir.y * dt * self.stats.velocity.y

		self.stats.lastVelocity.x = self.stats.velocity.x * dir.x
		self.stats.lastVelocity.y = self.stats.velocity.y * dir.y
	elseif self.state == "runningAway" then
		self.pos.x = self.pos.x + (-dir.x) * dt * self.stats.velocity.x
    	self.pos.y = self.pos.y + (-dir.y) * dt * self.stats.velocity.y

		self.stats.lastVelocity.x = self.stats.velocity.x * -dir.x
		self.stats.lastVelocity.y = self.stats.velocity.y * -dir.y
	end
end

function Defender:attackAI()
	if self.state == "attack" and
		((self.lastAttack ~= nill and
			self.lastAttack.timeDeath == true) or
		self.lastAttack == nil) then

		-- @TODO Come back when bullets are in.
		local bulVelocity = {
			x = 6 * TCE.lookAtVector(player.pos, self.pos).x,
			y = 6 * TCE.lookAtVector(player.pos, self.pos).y }
		
		self.lastAttack = Bullet.new(self.pos, 50, 10, 6, bulVelocity,1, player.pos, tex["greatSwordSwing"])
end
local TCE = require("data.scripts.tce.tce")

-- Essentially a moving tile but treated like an enemy.

SpikeBlock = setmetatable({}, {__index = Entity})
SpikeBlock.__index = SpikeBlock
function SpikeBlock.new(pos, directional)
	local self = setmetatable(
		Entity.new(
			pos, 20, 20, 0,
			{health = 0,
				maxHealth = 0,
				attack = 0, 
				velocity = {x = directional.x, y = directional.y, magnitude = 200},
				lastVelocity = {x = 0, y = 0},
				radius = 100,
				detectionGrace = TCE.MathConstants.M_PI / 6,
				lifeTime = 0.5}, -- 30 degrees
			{solid = true, 
				renderOn = true, 
				updateOn = true, 
				tag = "enemy",
				state = "waiting",
				upTime = 0},
			tex["spikeblock"]),
		SpikeBlock)

	self.originPosition = self.pos
	self.stats.velocity.angle = TCE.InnerProduct(self.stats.velocity, {x = 1, y = 0})

	self:uniqueIdAssign()

    entityList[self.id] = self
	renderList[self.id] = self
	return self
end

function SpikeBlock:update(dt, player)
	self:standardEnemyColCheck()

	-- AI
	self:stateCheck(dt, player)
	self:movement(dt, player)
	self:attackAI(player)

	if (self:onScreenCheck()) then 
		self.flags.renderOn = false
		self.flags.updateOn = false
		self.flags.solid = false
	end

	--print(self.flags.state)
end

function SpikeBlock:movement(dt, player)
	if self.flags.state == "attacking" then
		self.pos.x = self.pos.x + self.stats.velocity.x * dt * self.stats.velocity.magnitude
		self.pos.y = self.pos.y + self.stats.velocity.y * dt * self.stats.velocity.magnitude
	elseif self.flags.state == "returning" then
		self.pos.x = self.pos.x - self.stats.velocity.x * dt * self.stats.velocity.magnitude
		self.pos.y = self.pos.y - self.stats.velocity.y * dt * self.stats.velocity.magnitude
	end
end

function SpikeBlock:dealDmg(dmg)
	-- Can't be damaged.
end

function SpikeBlock:stateCheck(dt, target)
	-- Checks if in range
	local dir = TCE.LookAtVector(target.pos, self.pos)
	local angleDif = math.abs(TCE.InnerProduct(dir, self.stats.velocity))

	if self.flags.state == "attacking" then
		self.flags.upTime = self.flags.upTime + dt
	elseif self.flags.state == "returning" then
		self.flags.upTime = self.flags.upTime - dt
	end

	if (self.flags.upTime > self.stats.lifeTime) then
			self.flags.state = "returning"
	elseif angleDif < self.stats.detectionGrace / 2 and
		math.abs(dir.length) < self.stats.radius and
		self.flags.upTime <= 0 then
		self.flags.upTime = 0
		self.flags.state = "attacking"
		
	elseif self.flags.state ~= "attacking" and
		self.pos == self.originalPos then
		self.flags.state = "waiting"
	end
end

function SpikeBlock:attackAI(player)
	if self.flags.state == "attacking" then
		if checkColSpecific(player, self) then
			player:dealDmg(self.stats.attack)
			--self.flags.state = "returning"
		end
	end
end
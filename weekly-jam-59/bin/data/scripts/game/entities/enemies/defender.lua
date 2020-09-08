local TCE = require("data.scripts.tce.tce")

-- Stays a distance away from player and stabs. High health points and enemies
-- hide behind.

Defender = setmetatable({}, {__index = Entity})
Defender.__index = Defender
function Defender.new(pos)
	local self = setmetatable(
		Entity.new(
			pos,
			20, 20, 0,
			{health = 10,
		 	 	maxHealth = 10,
			 	attack = 6, 
			 	velocity = {x = 30, y = 30},
			 	lastVelocity = {x = 0, y = 0},
			 	stunDelay = {timer = 0, max = 2},
			 	radius = 60},
			{solid = true, 
			  	renderOn = true, 
			  	updateOn = true, 
			  	tag = "enemy",
			  	state = "rushing"},
			tex["defender"]),
		Defender)

	self:uniqueIdAssign()

    entityList[self.id] = self
	renderList[self.id] = self
	return self
end

function Defender:update(dt, player)
	self:stunTimer(dt)

	--print(self.flags.state)

	if self.flags.state ~= "stunned" then
		self:standardEnemyColCheck()
		
		-- AI
		self:stateCheck(player)
		self:movement(dt, player)
		self:attackAI(player)
		-- @INCOMPLETE ? Add a way for units to hide behind this unit? 
	end

	if (self:onScreenCheck()) then 
		self.flags.renderOn = false
		self.flags.updateOn = false
		self.flags.solid = false
	end
end

function Defender:stateCheck(target)
	local dir = TCE.LookAtVector(target.pos, self.pos)
	local offset = 20
	if self.stats.radius + offset < math.abs(dir.length) then
		self.flags.state = "rushing"
	elseif self.stats.radius > math.abs(dir.length) then
		self.flags.state = "runningAway"
	else 
		self.flags.state = "attack"
	end
end

function Defender:movement(dt, player)
	local dir = TCE.LookAtVector(player.pos, self.pos)
	if self.flags.state == "rushing" then
		self.pos.x = self.pos.x + dir.x * dt * self.stats.velocity.x
    	self.pos.y = self.pos.y + dir.y * dt * self.stats.velocity.y

		self.stats.lastVelocity.x = self.stats.velocity.x * dir.x
		self.stats.lastVelocity.y = self.stats.velocity.y * dir.y
	elseif self.flags.state == "runningAway" then
		self.pos.x = self.pos.x + (-dir.x) * dt * self.stats.velocity.x
    	self.pos.y = self.pos.y + (-dir.y) * dt * self.stats.velocity.y

		self.stats.lastVelocity.x = self.stats.velocity.x * -dir.x
		self.stats.lastVelocity.y = self.stats.velocity.y * -dir.y
	end
end

function Defender:attackAI(player)
	print("Defender Attacks.")
	-- Sword speed
	local magnitude = 50
	
	if (self.flags.state == "attack" or self.flags.state == "runningAway")and
		((self.lastAttack ~= nill and
			self.lastAttack.flags.timeDeath == true) or
		self.lastAttack == nil) then

		bulletWidth = 20
		bulletHeight = 40
		bulletDiagonal = (bulletWidth^2 + bulletHeight^2)^0.5

		bulVelocity = {
			x = magnitude * TCE.LookAtVector(self.pos, player.pos).x,
			y = magnitude * TCE.LookAtVector(self.pos, player.pos).y }

		normal = TCE.NormalizeVector(bulVelocity)

		bulBoxAngle = TCE.InnerProduct({x = 1, y = 0}, {x = bulletWidth, y = bulletHeight})
		bulletWidth = bulletDiagonal * math.cos(bulBoxAngle)
		bulletHeight = bulletDiagonal * math.sin(bulBoxAngle)
		

		posX = (self.pos.x + self.width/2) + bulletWidth * normal.x
		posY = (self.pos.y + self.height/2) + bulletHeight * normal.y
		self.lastAttack = Bullet.new(
			{x = posX, y = posY },
			20, 40,
			6, {x = normal.x*magnitude, y = normal.y*magnitude}, 10, 
			TCE.LookAtVector(player.pos, {x = posX, y = posY}),
			 tex["greatswordSlash"])
	end
end
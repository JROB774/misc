local TCE = require("data.scripts.tce.tce")

Snoteleks = setmetatable({}, {__index = Entity})
Snoteleks.__index = Snoteleks
function Snoteleks.new(pos)
	local self = setmetatable(
		Entity.new(
			pos,
			20, 20, 0,
			{health = 4,
		 	 	maxHealth = 4,
			 	attack = 6,
			 	velocity = {x = 50, y = 50},
			 	lastVelocity = {x = 0, y = 0},
			 	stunDelay = {timer = 0, max = 2},
			 	attackDelay = 1,
			 	movementDelay = 2},
			{solid = true,
			  	renderOn = true,
			  	updateOn = true,
			  	tag = "enemy",
			  	state = "attacking",
			  	attackCount = 1,
			  	attackTime = 0,
			  	movementTime = 0},
			tex["snoteleks"]),
		Snoteleks)

	self:uniqueIdAssign()
	self.STATE = "attacking"
	self.STATE = "attacking"
    entityList[self.id] = self
	renderList[self.id] = self
	return self
end

function Snoteleks:update(dt, player)
	self:stunTimer(dt)

	if self.STATE ~= "stunned" then
		self:standardEnemyColCheck()

		-- AI
		self:stateCheck(dt, player)
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

function Snoteleks:stateCheck(dt, player)
	if self.STATE == "moving" and
		self.flags.movementTime >= self.stats.movementDelay then

		self.flags.movementTime = 0
		self.STATE = "attacking"
		self.flags.attackCount = 3
	elseif self.STATE == "moving" then

		self.flags.movementTime = self.flags.movementTime + dt
	elseif self.STATE == "attacking" and
		self.flags.attackCount == 0 then

		self.STATE = "moving"

		-- Should be normalised but normalised causes nan issues ocasionally.
		self.stats.target = {x = math.random (-100, 100)/100, y = math.random (-100, 100)/100}
	elseif self.STATE == "attacking" and
		self.flags.attackCount > 0 then

		self.flags.attackTime = self.flags.attackTime + dt
	end
end

function Snoteleks:attackAI(player)
	if self.flags.attackTime > self.stats.attackDelay then
		self.flags.attackCount = self.flags.attackCount - 1
		self.flags.attackTime = 0

		-- @TODO change to parabola
		local magnitude = 50
		pos = {x = player.pos.x, y = player.pos.y}
		bulVelocity = {
			x = magnitude * TCE.LookAtVector(pos, self.pos).x,
			y = magnitude * TCE.LookAtVector(pos, self.pos).y }

		normal = TCE.NormalizeVector(bulVelocity)

		posX = ((self.pos.x + self.width/2) + self.width * normal.x)/TILE_WIDTH
		posY = ((self.pos.y + self.height/2) + self.height * normal.y)/TILE_HEIGHT
		--posX = self.pos.x/TILE_WIDTH
		--posY = self.pos.y/TILE_HEIGHT
		Bone.new({x = posX, y = posY}, 20, 20, 2, {x  = 5, y = 0, mag = 40}, 4, player.pos, tex["bone"])
	end
end

function Snoteleks:movement(dt, player)
	if self.STATE == "moving" then
    	self.pos.x = self.pos.x + self.stats.target.x * dt * self.stats.velocity.x
    	self.pos.y = self.pos.y + self.stats.target.y * dt * self.stats.velocity.y
	
		self.stats.lastVelocity.x = self.stats.velocity.x * self.stats.target.x
		self.stats.lastVelocity.y = self.stats.velocity.y * self.stats.target.y
	end
end

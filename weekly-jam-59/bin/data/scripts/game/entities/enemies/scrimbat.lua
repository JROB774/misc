local TCE = require("data.scripts.tce.tce")

-- Stays a distance away from player and stabs. High health points and enemies
-- hide behind.

Scrimbat = setmetatable({}, {__index = Entity})
Scrimbat.__index = Scrimbat

Scrimbat.WIDTH = 20
Scrimbat.HEIGHT = 20
Scrimbat.ANGLE = 0.0
Scrimbat.TEXTURE = tex["scrimbat"]


Scrimbat.STATE ={
	IDLE = 0,
	MOVE = 1,
	ATT = 2,

}

function Scrimbat.new(pos)
	local self = setmetatable(
		Entity.new(
			pos,
			Scrimbat.WIDTH,
			Scrimbat.HEIGHT,
			Scrimbat.ANGLE,
			{
				health = 5,
				maxHealth = 5,
				attack = 2,
				velocity = { x = 60, y = 60},
				lastVelocity = { x = 0, y = 0},
				stunDelay = { timer = 0, max = 2},
				radius = 60
			},
			{
				solid = true,
				updateOn = true,
				renderOn = true,
				tag = "enemy",
				state = ""
			},
			Scrimbat.TEXTURE),
		Scrimbat)

	self:uniqueIdAssign()

	self.target = {x = 0 , y = 0}
	self.dir = {x = 0, y = 0}
	self.state = Scrimbat.STATE.IDLE

	--This is how long it will stay at a desination or chase the player
	self.time = 0
	self.timeMax = 4

	self.zIndex    = Z_INDEX.ENTITY_SCRIMBAT

    entityList[self.id] = self
	renderList[self.id] = self
	return self
end

--Randomly decides whether to chase the player or roam
function Scrimbat:determineTarget(dt, player)

	-- 0 - 3 Roam // 4 Chase
	local temp = math.random(0, 5)

	-- Selects a random Posistion to Roam To
	if temp <= 3 then
		self.state = Scrimbat.STATE.MOVE
		local camera = TCE.GetCameraBounds()
		self.target.x = math.random(camera.x + 10, camera.x + camera.w - 10)
		self.target.y = math.random(camera.y + 10, camera.y + camera.h - 10)
		self.stats.velocity.x = 60
		self.stats.velocity.y = 60
	end

	-- Chases the Player for roughly 5 seconds
	if temp > 3 then
		self.state = Scrimbat.STATE.ATT
		self.target.x = player.pos.x
		self.target.y = player.pos.y
		self.stats.velocity.x = 90
		self.stats.velocity.y = 90
		self.time = self.timeMax
	end
end

function Scrimbat:movement(dt, player)

	--If the Entitys been idle for long enough
	if self.state == Scrimbat.STATE.IDLE then
		if self.time <= 0 then
			self:determineTarget(dt, player)
		end
	end

	-- Decide the posistion and the angle to chase
	local centerPos = { x = self.pos.x + (self.width / 2), y = self.pos.y + (self.height / 2) }
	local dir = TCE.LookAtVector(self.target, centerPos)

	--A2 + B2 = C2
	--Check if the Bats within 2 pixel range of its spot if so IDLE and start counting
	if math.sqrt((centerPos.x^2) + (centerPos.y^2)) - math.sqrt((self.target.x^2) + (self.target.y^2)) > -2 then
		if math.sqrt((centerPos.x^2) + (centerPos.y^2)) - math.sqrt((self.target.x^2) + (self.target.y^2)) < 2 then
			if self.state == Scrimbat.STATE.MOVE then
				self.state = Scrimbat.STATE.IDLE
				self.time = 3
			end
		end
	end


	--Check the time on chasing the player if the timers above 0 update the target pos
	if self.time >= 0 and self.state == Scrimbat.STATE.ATT then
		self.target.x = player.pos.x
		self.target.y = player.pos.y
	end
	if self.time <= 0 and self.state == Scrimbat.STATE.ATT then
		self.state = Scrimbat.STATE.IDLE
	end

	-- If the bats idle stop moving, (Stops it gittering around)
	if self.state ~= Scrimbat.STATE.IDLE then
		self.pos.x = self.pos.x + dir.x * dt * self.stats.velocity.x
  		self.pos.y = self.pos.y + dir.y * dt * self.stats.velocity.y
  	end

  	--Timer
  	self.time = self.time - dt
end

function Scrimbat:update(dt,player)
	--self:standardEnemyColCheck()
	self:movement(dt, player)

	if self:onScreenCheck() then
		self.flags.renderOn = false
		self.flags.updateOn = false
		self.flags.solid    = false
	end
end

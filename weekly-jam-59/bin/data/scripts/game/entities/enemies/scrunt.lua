local TCE = require("data.scripts.tce.tce")

Scrunt = setmetatable({}, { __index = Entity })
Scrunt.__index = Scrunt

-- ENTITY VALUES ---------------------------------------------------------------

Scrunt.WIDTH   = 20
Scrunt.HEIGHT  = 35
Scrunt.ANGLE   = 0.0
Scrunt.TEXTURE = tex["scrunt"]
Scrunt.STATS   =
{
	health       = 1,
	maxHealth    = 1,
	attack       = 0,
	velocity     = { x = 0, y = 0 },
	lastVelocity = { x = 0, y = 0 },
	stunDelay    = { timer = 0, max = 2 }
}
Scrunt.FLAGS   =
{
	solid        = true,
	updateOn     = true,
	renderOn     = true,
	tag          = "enemy",
	state        = ""
}

--------------------------------------------------------------------------------

-- SCRUNT VALUES ---------------------------------------------------------------

Scrunt.MIN_MOVE_TIME = 0.35
Scrunt.MAX_MOVE_TIME = 1.50
Scrunt.MIN_WAIT_TIME = 0.90
Scrunt.MAX_WAIT_TIME = 2.40

Scrunt.SPEED = TILE_WIDTH * 1.5

Scrunt.STATE =
{
	IDLE = 0,
	MOVE = 1,
	WAIT = 2
}

-------------------------------------------------------------------------------------

function Scrunt.new (pos)
	local self = setmetatable(
		Entity.new(
			pos,
			Scrunt.WIDTH,
			Scrunt.HEIGHT,
			Scrunt.ANGLE,
			TCE.DeepCopyTable(Scrunt.STATS),
			TCE.DeepCopyTable(Scrunt.FLAGS),
			Scrunt.TEXTURE
		),
		Scrunt
	)

	self.zIndex    = Z_INDEX.ENTITY_BASIC
	self.dir       = { x = 0, y = 0 }
	self.moveTime  = 0.0
	self.waitTime  = 0.0
	self.state     = Scrunt.STATE.IDLE

	self:uniqueIdAssign()
	print("created")

	entityList[self.id] = self
	renderList[self.id] = self

	return self
end

function Scrunt:determineTarget (dt)
	-- He thinks about what direction he wants to move in and for how long.
	self.dir = TCE.RotateVector({ x = 1.0, y = 0.0}, math.rad(math.random(0, 360)))
	self.moveTime = TCE.RealNumberRandom(Scrunt.MIN_MOVE_TIME, Scrunt.MAX_MOVE_TIME)

	-- Now he is focused solely on moving to his target.
	self.state = Scrunt.STATE_MOVE
end

function Scrunt:moveTowardsTarget (dt)
	-- He slowly moves towards his chosen destination whilst counting in his head.
	local centerPos = { x = self.pos.x + (self.width / 2), y = self.pos.y + (self.height / 2) }
	local targetPos = { x = centerPos.x + self.dir.x, y = centerPos.y + self.dir.y }

	local dir = TCE.LookAtVector(targetPos, centerPos)

  self.pos.x = self.pos.x + dir.x * dt * Scrunt.SPEED
  self.pos.y = self.pos.y + dir.y * dt * Scrunt.SPEED

  -- Counting in his head until he should wait for a bit.
  self.moveTime = self.moveTime - dt
  if self.moveTime <= 0.0 then
  	-- Once done, he now thinks about how long he should wait for.
  	self.waitTime = TCE.RealNumberRandom(Scrunt.MIN_WAIT_TIME, Scrunt.MAX_WAIT_TIME)
  	self.state = Scrunt.STATE.WAIT
  end
end

function Scrunt:waitAround (dt)
	-- Counting in his head until he should start moving again.
	self.waitTime = self.waitTime - dt
	if self.waitTime <= 0.0 then self.state = Scrunt.STATE.IDLE end
end

function Scrunt:movement (dt, player)
	-- Based on his current state, carry out a particular behaviour.
	if self.state == Scrunt.STATE.IDLE then self:determineTarget(dt)
	elseif self.state == Scrunt.MOVE then self:moveTowardsTarget(dt)
	else self:waitAround(dt) end
end

function Scrunt:update (dt, player)
	-- Carries out his behaviour and then checks if he's colliding.
	self:movement(dt, player)
	self:standardEnemyColCheck()

	-- If he's off-screen (not in the room) then he goes away.
	if self:onScreenCheck() then
		self.flags.renderOn = false
		self.flags.updateOn = false
		self.flags.solid    = false
	end
end

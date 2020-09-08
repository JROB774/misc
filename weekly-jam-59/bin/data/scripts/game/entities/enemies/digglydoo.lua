local TCE = require("data.scripts.tce.tce")

Digglydoo = setmetatable({}, { __index = Entity })
Digglydoo.__index = Digglydoo

-- ENTITY VALUES ---------------------------------------------------------------

Digglydoo.WIDTH   = 20
Digglydoo.HEIGHT  = 20
Digglydoo.ANGLE   = 0.0
Digglydoo.TEXTURE = tex["digglydoo"]
Digglydoo.STATS   =
{
	health          = 1,
	maxHealth       = 1,
	attack          = 0,
	velocity        = { x = 0, y = 0 },
	lastVelocity    = { x = 0, y = 0 },
	stunDelay       = { timer = 0, max = 2 }
}
Digglydoo.FLAGS   =
{
	solid           = true,
	updateOn        = true,
	renderOn        = true,
	tag             = "enemy",
	state           = ""
}

--------------------------------------------------------------------------------

-- DIGGLYDOO VALUES ------------------------------------------------------------

Digglydoo.START_WAIT_TIME   = 1.00
Digglydoo.MIN_WAIT_INTERVAL = 0.05
Digglydoo.MAX_WAIT_INTERVAL = 0.90

Digglydoo.STATE =
{
	WAIT  = 0,
	SHOOT = 1,
	MOVE  = 2,
}

Digglydoo.STATE_TO_ANIMATION =
{
	[Digglydoo.STATE.WAIT]  = "digglydoo_wait",
	[Digglydoo.STATE.SHOOT] = "digglydoo_shoot",
	[Digglydoo.STATE.MOVE]  = "digglydoo_move",
}

--------------------------------------------------------------------------------

function Digglydoo.new (pos)
	local self = setmetatable(
		Entity.new(
			pos,
			Digglydoo.WIDTH,
			Digglydoo.HEIGHT,
			Digglydoo.ANGLE,
			TCE.DeepCopyTable(Digglydoo.STATS),
			TCE.DeepCopyTable(Digglydoo.FLAGS),
			Digglydoo.TEXTURE
		),
		Digglydoo
	)

	self.zIndex   = Z_INDEX.ENTITY_BASIC
	self.waitTime = Digglydoo.START_WAIT_TIME
	self.state    = Digglydoo.STATE.WAIT

	self:uniqueIdAssign()

	entityList[self.id] = self
	renderList[self.id] = self

	return self
end

function Digglydoo:waitAround (dt, player)
	-- He waits around for a bit whilst thinking what he should do.
	self.waitTime = self.waitTime - dt

	-- If he's waited long enough he decided whether to: wait more, go down, or shoot.
	if self.waitTime <= 0.0 then
		self.state = math.random(Digglydoo.STATE.WAIT, Digglydoo.STATE.MOVE)
		-- If he decided to wait some more then he decides how long for
		if self.state == Digglydoo.STATE.WAIT then
			self.waitTime = TCE.RealNumberRandom(Digglydoo.MIN_WAIT_INTERVAL, Digglydoo.MAX_WAIT_INTERVAL)
		end
	end
end

function Digglydoo:moveAroundUnderground (dt, player)
	self.state = Digglydoo.STATE.WAIT
end

function Digglydoo:shootAtPlayer (dt, player)
	self.state = Digglydoo.STATE.WAIT
end

function Digglydoo:movement (dt, player)
	-- Based on his current state, carry out a particular behaviour.
	if self.state == Digglydoo.STATE.WAIT then self:waitAround(dt, player)
	elseif self.state == Digglydoo.STATE.SHOOT then self:shootAtPlayer(dt, player)
	else self:moveAroundUnderground(dt, player) end
end

function Digglydoo:update (dt, player)
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

function Digglydoo:render ()
	-- Draws the Digglydoo in his current animation state.
	local currentAnimation = anm[Digglydoo.STATE_TO_ANIMATION[self.state]]
	TCE.RenderAnimatedTexture(self.texture, currentAnimation, { x = self.pos.x, y = self.pos.y })
end

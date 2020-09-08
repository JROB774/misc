local TCE = require("data.scripts.tce.tce")

Entity = {}
Entity.__index = Entity
setmetatable(Entity, {__call = function (cls, ...) return cls.new(...) end})

--- Minimum variable requirements.
--- pos = {x, y}
--- stats = {attack, velocity = {x, y}}
--- flags = {renderOn, updateOn, solid, state}
Entity.pos = {x = 0, y = 0}
Entity.width = TILE_WIDTH
Entity.height = TILE_HEIGHT
Entity.angle = 0
Entity.zIndex = 0
Entity.id = 0
Entity.stats = {health = 0,
				  maxHealth = 0,
				  attack = 0,
				  velocity = {x = 0, y = 0},
				  lastVeclocity = {x = 0, y = 0},
				  stunDelay = {timer = 0, max = 2}}
Entity.flags = {renderOn = true,
				  updateOn = true,
				  solid = true,
				  tag = "enemy",
				  state = ""}

--- STATICS
-------------------------------------------------------------------------------

Entity.count = 1

Entity.debugSortIterations   = 0
Entity.debugListLength       = 0
Entity.debugActiveListLength = 0

Entity.sortRenderList = function (renderList)
	-- Get the debug information to be printed later in debug.lua.
	Entity.debugSortIterations   = 0
	Entity.debugListLength       = TCE.TableLength(renderList)
	Entity.debugActiveListLength = TCE.TableLengthWhereItem(renderList, function (i) return i.flags.renderOn end)

	-- Sorts the draw-order of the entire render list contents.0
	table.sort(renderList, function (a, b)
		Entity.debugSortIterations = Entity.debugSortIterations + 1

		local aZ = a.zIndex + TCE.Normalize(a.pos.y, 0, TCE.GetViewportHeight())
		local bZ = b.zIndex + TCE.Normalize(b.pos.y, 0, TCE.GetViewportHeight())

		-- This if prevents flashing when two objects share height.
		if aZ == bZ then
			return a.pos.x < b.pos.x
		end
		return aZ < bZ
	end)
end

-------------------------------------------------------------------------------

function Entity.new(pos, width, height, angle, stats, flags, texture)
	local self = setmetatable({
		pos = {
			x = pos.x * TILE_WIDTH,
			y = pos.y * TILE_HEIGHT},
		width = width,
		height = height,
		zIndex = Z_INDEX.ENTITY_BASIC,
		angle = angle,
		stats = stats,
		flags = flags,
		texture = texture,
		collisions = {},
		id = 0
		}, Entity)

	--renderList[self.id] = self
	return self
end

-- SYSTEM METHODS
-------------------------------------------------------------------------------
function Entity:update(dt)
	if (self:onScreenCheck()) then
		self.flags.renderOn = false
		self.flags.updateOn = false
		self.flags.solid = false
	end
	self:stunTimer(dt)
end

function Entity:render()
	if (self.stats.velocity.x >= 0) then
	  TCE.RenderTexture(self.texture, {x = self.pos.x, y = self.pos.y, angle = 0})
  elseif (self.stats.velocity.x < 0) then
    TCE.RenderTexture(self.texture, {x = self.pos.x, y = self.pos.y, angle = 0, flip = TCE.RenderFlips.HORZ})
  end
end

function Entity:uniqueIdAssign()
	self.id = Entity.count
	Entity.count = Entity.count + 1
end
-------------------------------------------------------------------------------

function Entity:movementRebound(collided)
    -- These just need to be a very large number by default.
    local xInterp = 1000000
    local yInterp = 1000000

    if (self.pos.x < collided.pos.x) then xInterp = -((self.pos.x + self.width) - collided.pos.x)
    else xInterp = ((collided.pos.x + collided.width) - self.pos.x) end
    if (self.pos.y < collided.pos.y) then yInterp = -((self.pos.y + self.height) - collided.pos.y)
    else yInterp = ((collided.pos.y + collided.height) - self.pos.y) end

    -- It is important that the equal to case does not get handled!
    if math.abs(xInterp) > math.abs(yInterp) then
        self.pos.y = self.pos.y + yInterp
        --self.stats.velocity.y = 0.0
    elseif math.abs(xInterp) < math.abs(yInterp) then
        self.pos.x = self.pos.x + xInterp
        --self.stats.velocity.x = 0.0
    end
end

function Entity:dealDmg(dmg)
	if (dmg) > 0 and self.flags.state ~= "stunned" then
    	self.stats.health = self.stats.health - dmg
   		self.flags.state = "stunned"
    elseif (dmg) < 0 and self.stats.health - dmg < self.stats.maxHealth then
    	self.stats.health = self.stats.health - dmg
    end

    if self.stats.health <= 0 then
        self.flags.solid = false;
        self.flags.renderOn = false;
        self.flags.updateOn = false;
    end
end

function Entity:stunTimer(dt)
	if self.stats.stunDelay.timer < self.stats.stunDelay.max and
		self.flags.state == "stunned" then
		self.stats.stunDelay.timer = self.stats.stunDelay.timer + dt
		self.flags.solid = false
	else
		self.stats.stunDelay.timer = 0
		self.flags.state = ""
		self.flags.solid = true
	end
end

function Entity:standardEnemyColCheck()
	self.collisions = checkColWithList(self, tileList)
	for key in pairs(self.collisions) do
		self:movementRebound(self.collisions[key])
	end

	if player.flags.solid == true then
		if checkColSpecific(player, self) then
				player:dealDmg(self.stats.attack)
				--self:movementRebound(player)
			end

		self.collisions = {}
	end
end

function Entity:onScreenCheck(graceX, graceY)
	graceX = graceX or 40
	graceY = graceY or 40

	local camera = TCE.GetCameraBounds()
	if self.pos.x < camera.x + 0        - graceX or
		 self.pos.x > camera.x + camera.w + graceX or
		 self.pos.y < camera.y + 0        - graceY or
		 self.pos.y > camera.y + camera.h + graceY then
		return true
	else
		return false
	end
end

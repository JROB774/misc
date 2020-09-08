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
Entity.id = 0
Entity.stats = {health = 0, 
				  attack = 0, 
				  velocity = {x = 0, y = 0},
				  lastVeclocity = {x = 0, y = 0}}
Entity.flags = {renderOn = true,
				  updateOn = true,
				  solid = true,
				  tag = "enemy",
				  state = ""}
function Entity.new(pos, width, height, angle, stats, flags, tag, texture)
	local self = setmetatable({
		pos = pos,
		width = width,
		height = height,
		angle = angle,
		stats = stats,
		flags = flags,
		flags.tag = tag,
		texture = texture,
		collisions = {},
		}, Entity)

	table.insert(renderList, self)

	return self
end

-- SYSTEM METHODS
-----------------
function Entity:update(dt)
	if (self:onScreenCheck) then 
		self.flags.renderOn = false
		self.flags.updateOn = false
	end
end

function Entity:render()
	if (self.stats.velocity.x > 0) then
	   TCE.RenderTexture(self.texture, {x = self.pos.x, y = self.pos.y, angle = 0})
    elseif (self.stats.velocity.x < 0) then
        TCE.RenderTexture(self.texture, {x = self.pos.x, y = self.pos.y, angle = 0, flip = TCE.RenderFlips.HORZ})
    end
end
-------------------

function Entity:movementRebound(collided)
    -- @TODO:  maybe make this a proper high constant at some point???
    xInterp = 1000000
    yInterp = 1000000

    if (self.pos.x < collided.pos.x) then xInterp = -((self.pos.x + self.width) - collided.pos.x)
    else xInterp = ((collided.pos.x + collided.width) - self.pos.x) end

    if (self.pos.y < collided.pos.y) then yInterp = -((self.pos.y + self.height) - collided.pos.y)
    else yInterp = ((collided.pos.y + collided.height) - self.pos.y) end

    xInterp = TCE.Round(xInterp)
    yInterp = TCE.Round(yInterp)

    if (math.abs(xInterp) > math.abs(yInterp)) then
        self.pos.y = self.pos.y + yInterp
        self.speed.y = 0.0
    else
        self.pos.x = self.pos.x + xInterp
        self.speed.x = 0.0
    end
end

function Entity:dealDmg(dmg)
    self.stats.health = self.stats.health - dmg
    if self.stats.health <= 0 then
        self.flags.solid = false;
        self.flags.renderOn = false;
        self.flags.updateOn = false;
    end
end

function Entity:standardEnemyColCheck()
	self.collisions = TCE.checkColWithList(self, tileList)
	for key in pairs(self.collisions) do
		self:movementRebound(self.collision[key])
	end

	if TCE.checkColSpecific(player, self) then
		player:dealDmg(self.attack)
		self:movementRebound(player)
	end

	self.collisions = {}
end

function Entity:onScreenCheck(grace = 20)
	if self.pos.x < 0 - grace or
		self.pos.x > TCE.GetViewportWidth() + grace or
		self.pos.y < 0 - grace or
		self.pos.y > TCE.GetViewportHeight() + grace then

		return true
	end
end
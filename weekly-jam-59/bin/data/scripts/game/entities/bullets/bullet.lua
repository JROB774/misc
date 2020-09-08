local TCE = require("data.scripts.tce.tce")

--- The base class represents enemy bullets.
Bullet = setmetatable({}, { __index = Entity })
Bullet.__index = Bullet
function Bullet.new(pos, width, height, attack, velocity, lifeTime, target, texture)
	local self = setmetatable(
		Entity.new(
			pos, height, width, (math.atan(velocity.y, velocity.x)), --+ TCE.MathConstants.M_PI/2),
			{health = 10,
                attack = attack,
                velocity = velocity,
                lastVelocity = {x = lastVelocity, y = lastVelocity},
                lifeTime = lifeTime,
                upTime = 0},
			{solid = true,
                renderOn = true,
                updateOn = true,
                tag = "bullet",
                state = "",
                timeDeath = false},
			texture),
		Bullet)


    -- Required as pass by reference shizzle
	self.target = {x = target.x, y = target.y}
	self.launchPos = pos
    self.zIndex = Z_INDEX.BULLET

    self:uniqueIdAssign()
    entityList[self.id] = self
    renderList[self.id] = self
	return self
end

function Bullet:update(dt, player)
    if self.flags.state ~= "death" then
        if (self.flags.timeDeath == false) then
            self:movement(dt)
            if self.flags.solid then
                self.collisions = checkColWithList(self, tileList)
                for key in pairs(self.collisions) do
                    self:selfDestruct()
                end
                if checkColSpecific(self, player) then
                    self:selfDestruct()
                    player:dealDmg(self.stats.attack)
                end
            end
        end
        self:ageCheck(dt)

        if(self:onScreenCheck()) then
            self:selfDestruct()
            self.flags.timeDeath = true
            self.flags.updateOn = false
        end
    end
end


function Bullet:render()
    TCE.RenderTexture(self.texture, {x = self.pos.x, y = self.pos.y, angle = self.angle})
end

function Bullet:movement(dt)
    -- IDK WHY THIS NEEDS TO BE HERE INSTEAD OF IN NEW. IDC
    local dir = TCE.NormalizeVector(TCE.LookAtVector(self.target, { x =self.launchPos.x * TILE_WIDTH, y = self.launchPos.y * TILE_HEIGHT}))
    self.pos.x = self.pos.x + dir.x * dt * self.stats.velocity.x
    self.pos.y = self.pos.y + dir.y * dt * self.stats.velocity.y
end

function Bullet:movementRebound(collided)
    -- pass
end

function Bullet:selfDestruct()
    self.flags.solid = false
    self.flags.renderOn = false
    --entityList[self.id] = nil
    renderList[self.id] = nil
    --- @TODO Pool systems
    collectgarbage()
end

function Bullet:ageCheck(dt)
    self.stats.upTime = self.stats.upTime + dt
    if (self.stats.upTime > self.stats.lifeTime) then
        -- self.flags.state = "death"
        self:selfDestruct()
        self.flags.timeDeath = true
        self.flags.updateOn = false
    end
end

local TCE = require("data.scripts.tce.tce")

--- The base class represents enemy bullets.
Bullet = setmetatable({}, { __index = Entity })
Bullet.__index = Bullet
function Bullet.new(pos, width, height, attack, velocity, lifeTime, target, texture)
	local self = setmetable(
		Entity.new(
			pos, height, width, math.atan(velocity.y, velocity.x),
			{health = 10, 
			  attack = attack, 
			  velocity = velocity,
			  lastVelocity = {x = lastVelocity, y = lastVelocity}}),
			{solid = true, 
			  renderOn = true, 
			  updateOn = true, 
			  tag = "bullet",
			  state = ""},
			texture),
		Bullet)

	self.stats.lifeTime = lifeTime
	self.stats.upTime = 0
	self.flags.timeDeath = false

	self.target = target
	self.launchPos = pos

	table.insert(entityList, self)
	table.insert(renderList, self)

	return self
end

function Bullet:update(dt, player)
	if (self.flags.timeDeath == false) then
        self:movement(dt)
        if self.flags.solid then 
        	self.collisions = TCE.checkColWithList(self, tileList)
        	for key in pairs(self.collisions) do
        	    self:selfDestruct()
        	end
        	self.collisions = TCE.checkColWithList(self, entityList)
            for key in pairs(self.collisions) do
                if self.collisions[key].tag ~= "goodBullet" then
                    self:selfDestruct()
                    if (self.collisions[key].flags.tag == "player") then
                        self.collisions[key]:dealDmg(self.attack)
                    end
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

function Bullet:movement(dt)
    local dir = TCE.lookAtVector(self.target, self.launchPos)
    self.pos.x = self.pos.x + dir.x * dt * self.stats.velocity.x
    self.pos.y = self.pos.y + dir.y * dt * self.stats.velocity.y
end

function Bullet:movementRebound(collided)
    -- pass
end

function Bullet:selfDestruct()
    self.flags.solid = false
    self.flags.renderOn = false
end

function Bullet:ageCheck(dt)
    self.stats.upTime = self.stats.upTime + dt
    if (self.stats.upTime > self.stats.lifeTime) then
        print("A timely death")
        self:selfDestruct()
        self.flags.timeDeath = true
        self.flags.updateOn = false
    end
end
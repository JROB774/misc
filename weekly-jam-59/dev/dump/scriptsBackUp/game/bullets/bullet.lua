local TCE = require("data.scripts.tce.tce")

Bullet = setmetatable({}, { __index = Entity })
Bullet.__index = Bullet

function Bullet.new (x, y, width, height, attack, speed, lifeTime, target, file, tag, player)
    local self = setmetatable(
        Entity.new(x, y, width, height, health, attack, speed, math.tan(target.y, target.x), true, true, true, file, tag),
        Bullet)

    self.lifeTime = lifeTime
    self.upTime = 0

    self.target = target
    self.launchPos = {x = self.pos.x, y = self.pos.y}

    self.speed.magnitude = speed
    self.lastVelocity.x = self.speed.x
    self.lastVelocity.y = self.speed.y

    self.timeDeath = false

    return self
end

function Bullet:update(dt, player)
    -- pass
end

function Bullet:movement(dt)
    local dir = self:lookAtVector(self.target, self.launchPos)
    self.pos.x = self.pos.x + dir.x * dt * self.speed.x
    self.pos.y = self.pos.y + dir.y * dt * self.speed.y

end

function Bullet:movementRebound(collided)
    -- pass
end

function Bullet:selfDestruct()
    self.solid = false
    self.renderOn = false
end

function Bullet:ageCheck(dt)
    self.upTime = self.upTime + dt
    if (self.upTime > self.lifeTime) then
        print("A timely death")
        self:selfDestruct()
        self.timeDeath = true
        self.updateOn = false
    end
end
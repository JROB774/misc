local TCE = require("data.scripts.tce.tce")

--- Snotelek bullet. Parabola style tosses
Bone = setmetatable({}, { __index = Entity })
Bone.__index = Bone
function Bone.new(pos, width, height, attack, velocity, lifeTime, target, texture)
	local self = setmetatable(
		Bullet.new(pos, width, height, attack, velocity, lifeTime, target, texture),
		Bone)

    -- Required as pass by reference shizzle
	self.target = {x = target.x, y = target.y}
	self.launchPos = pos
    self.zIndex    = Z_INDEX.ENTITY_BASIC + 10
    self.stats.height = 5

    ------------ WORKING
    --self.stats.vertex = {h = (self.target.x/TILE_WIDTH - self.launchPos.x/TILE_WIDTH)/2, k = (self.launchPos.y/TILE_HEIGHT + self.stats.height/TILE_HEIGHT) }
    
    self.stats.vertex = {
        h = (self.target.x/TILE_WIDTH + self.stats.height),
        k = (self.target.x/TILE_WIDTH + self.stats.height)}
    self.stats.focus = {x = self.stats.vertex.h, y = self.stats.vertex.k + 1/(4*1)}

    xAdjusted = self.pos.x/TILE_WIDTH
    self.halfDistance = xAdjusted + (self.target.x/TILE_WIDTH - xAdjusted)/2
    self.t = TCE.InnerProduct(self.target, {x = 1, y = 0})
  

    ------- SUVAT equations
    ---------------------------------------------------------------------------
    --self.stats.lifeTime = (self.target.x - self.launchPos.x)/self.stats.velocity.x
   -- self.stats.velocity.y = self.stats.velocity.mag * math.cos(self.stats.velocity.x)
    self:uniqueIdAssign()
    entityList[self.id] = self
    renderList[self.id] = self
	return self
end

function Bone:update(dt, player)
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

        xAdjusted = self.pos.x/TILE_WIDTH
       --[[ if(TCE.RoundNum(self.pos.y,1) == TCE.RoundNum(self.target.y, 1)) then
--            print("do I happen?")
            self.flags.state = "death"
            self:selfDestruct()
        end]]

        if(self:onScreenCheck()) then
            self:selfDestruct()
            self.flags.timeDeath = true
            self.flags.updateOn = false
        end
    end
end

function Bone:movement(dt)
    -- IDK WHY THIS NEEDS TO BE HERE INSTEAD OF IN NEW. IDC
    
    local dir = TCE.NormalizeVector(TCE.LookAtVector(self.target, { x =self.launchPos.x * TILE_WIDTH, y = self.launchPos.y * TILE_HEIGHT}))


    self.pos.x = self.pos.x + dir.x * dt * TILE_WIDTH --self.stats.velocity.x * TILE_WIDTH
    xAdjusted = self.pos.x/TILE_WIDTH
    self.pos.y = self.pos.y + dt*2*(xAdjusted - self.halfDistance)*(self.stats.height)

    self.pos.y = self.pos.y + (xAdjusted - self.stats.focus.x)/(-self.stats.vertex.k)*dt*self.stats.height


    --self.pos.x = self.pos.x + dir.x * dt * TILE_WIDTH --self.stats.velocity.x * TILE_WIDTH

    --print(- self.stats.velocity.y/TILE_HEIGHT * self.stats.upTime  + 9.8/TILE_HEIGHT * self.stats.upTime^2)
    --self.pos.y = self.launchPos.y - self.stats.velocity.y/TILE_HEIGHT  + 9.8/TILE_HEIGHT * self.stats.upTime

end

--[[
local dir = TCE.NormalizeVector(TCE.LookAtVector(self.target, { x =self.launchPos.x * TILE_WIDTH, y = self.launchPos.y * TILE_HEIGHT}))

    self.pos.x = self.pos.x + dir.x * dt * TILE_WIDTH --self.stats.velocity.x * TILE_WIDTH
    xAdjusted = self.pos.x/TILE_WIDTH
    self.pos.y = self.pos.y + dt*2*(xAdjusted - self.halfDistance)*(self.stats.height)
    ]]

function Bone:movementRebound(collided)
    -- pass
end

function Bone:selfDestruct()
    self.flags.solid = false
    self.flags.renderOn = false
    --entityList[self.id] = nil
    renderList[self.id] = nil
    --- @TODO Pool systems
    collectgarbage()
end

function Bone:ageCheck(dt)
    self.stats.upTime = self.stats.upTime + dt
    if (self.stats.upTime > self.stats.lifeTime) then
        self.flags.state = "death"
    end
end



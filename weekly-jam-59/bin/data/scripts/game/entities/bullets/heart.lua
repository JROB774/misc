local TCE = require("data.scripts.tce.tce")

--- Your basic good boi conversion tool.
Heart = setmetatable({}, { __index = Bullet })
Heart.__index = Heart
function Heart.new(pos, target)
	local self = setmetatable(
		Bullet.new(pos, 10, 10, 5, {x = 160, y = 160 }, 0.5, target, tex["love"]),
		Heart)

    -- How fast the bullet loses velocity.
    self.fallOffTime =  0.00
    self.fallOffRate =  0.05
    self.fallOff     = 10.00

    self.flags.tag = "heart"
	self.target = target
    self.launchPos = pos
    self.pos = {x = self.pos.x - self.width/2, y = self.pos.y - self.height/2}

    self.zIndex = Z_INDEX.BULLET
    self:uniqueIdAssign()

    entityList[self.id] = self
    renderList[self.id] = self
	return self
end

function Heart:update(dt, player)
    if self.flags.state ~= "death" then
        if (self.flags.timeDeath == false) then
            self:movement(dt)
            if(self.flags.solid == true) then
                self.collisions = checkColWithList(self, tileList)
                for key in pairs(self.collisions) do
                    self:selfDestruct()
                end
                self.collisions = checkColWithList(self, entityList)
                for key in pairs(self.collisions) do
                    if (self.collisions[key].flags.tag ~= "heart") then
                        self:selfDestruct()
                        if (self.collisions[key].flags.tag == "enemy") then
                            self.collisions[key]:dealDmg(self.stats.attack)
                        end
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

        -- Makes the bullet lose velocity over time.
        self.fallOffTime = self.fallOffTime + dt
        while self.fallOffTime >= self.fallOffRate do
            self.stats.velocity.x = self.stats.velocity.x - self.fallOff
            self.stats.velocity.y = self.stats.velocity.y - self.fallOff
            self.fallOffTime = self.fallOffTime - self.fallOffRate

            if self.stats.velocity.x <= 0.0 then self.stats.velocity.x = 0.0 end
            if self.stats.velocity.y <= 0.0 then self.stats.velocity.y = 0.0 end
        end
    else
        if TCE.AnimationIsDone(anm["love_death"]) then
            TCE.ResetAnimation(anm["love_death"])
            self:selfDestruct()
            self.flags.timeDeath = true
            self.flags.updateOn = false
        end
    end
end

function Heart:render()
    local animation = anm["love"]
    if self.flags.state == "death" then animation = anm["love_death"] end
    TCE.RenderAnimatedTexture(self.texture, animation, {x = self.pos.x, y = self.pos.y  })
end

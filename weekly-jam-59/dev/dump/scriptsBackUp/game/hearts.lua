local TCE = require("data.scripts.tce.tce")

--- Your basic good boi conversion tool.
Heart = setmetatable({}, { __index = Bullet })
Heart.__index = Heart
function Bullet.new(pos, target)
	local self = setmetable(
		Heart.new(pos, 20, 20, 2, {x = 500, y = 500}, 2, target, tex["heartBullet"]),
		Heart)
	self.stats.tag = "heart"
	
	return self
end

function HeartBullet:update(dt, player)
    if self.flags.timeDeath == false then
        self:movement(dt)
    
        if self.flags.solid == true then
            self.collision = TCE.checkColWithList(self, tileList)
            for key in pairs(self.collisions) do
                self:selfDestruct()
            end
            self.collisions = TCE.checkColWithList(self, entityList)
            for key in pairs(self.collisions) do
                if self.collisions[key].tag ~= "goodBullet" then
                    self:selfDestruct()
                    if (self.collisions[key].flags.tag == "enemy") then
                        self.collisions[key]:dealDmg(self.attack)
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
end
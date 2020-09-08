local TCE = require("data.scripts.tce.tce")

-- Your Basic Good boi bullet.

HeartBullet = setmetatable({}, {__index = Bullet})
HeartBullet.__index = HeartBullet
function HeartBullet.new(x, y, attack, speed, target, player)
	local self = setmetatable(
		Bullet.new(x, y, TILE_WIDTH, TILE_HEIGHT, attack, speed, 6, target, tex["heartBullet"], "goodBullet", player),
		HeartBullet)
end

function HeartBullet:update(dt, player)
    if (self.timeDeath == false) then
        self:movement(dt)
    
        if(self.solid == true) then
            self:checkColTiles()
            for key in pairs(self.collisions) do
                self:selfDestruct()
            end
            self:checkEntityTiles()
            for key in pairs(self.collisions) do
                if (self.collisions[key].tag ~= "goodBullet") then
                    self:selfDestruct()
                    if (self.collisions[key].tag == "enemy") then
                        self.collisions[key]:dealDmg(self.attack)
                    end
                end
            end
        end
    end
    self:ageCheck(dt)
end

local TCE = require("data.scripts.tce.tce")

-- Your Basic Good boi bullet.

EnemyBullet = setmetatable({}, {__index = Bullet})
EnemyBullet.__index = EnemyBullet
function EnemyBullet.new(x, y, width, height, attack, speed, age, target, file, player)
	local self = setmetatable(
		Bullet.new(x, y, width, height, attack, speed, age, target, file, "badBullet", player),
		EnemyBullet)
end

function EnemyBullet:update(dt, player)
    if (self.timeDeath == false) then
        self:movement(dt)
        if(self.solid == true) then
            self:checkColTiles()
            for key in pairs(self.collisions) do
                self:selfDestruct()
            end
        
            if (self.checkColSpecific(self, player)) then
                self:selfDestruct()
                player:dealDmg(self.attack)
            end
        end
    end
    self:ageCheck(dt)

end
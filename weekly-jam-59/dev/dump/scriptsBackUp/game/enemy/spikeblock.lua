local TCE = require("data.scripts.tce.tce")

-- Four cardinal directions rush Enemy that deals damage on contact. Only
-- rushes when in line of sight.

Spikeblock = setmetatable({}, {__index = Entity})
Spikeblock.__index = Spikeblock
function Spikeblock.new(x, y, angle)
	local self = setmetatable(
		Entity.new(x, y, TILE_WIDTH, TILE_HEIGHT, 2, 1, 35, 0, true, true, true, tex["spikeblock"], enemy),
		Spikeblock)

	self.speed.x = 0;
	self.speed.y = 0;
	self.moving = false;

	return self
end

function Spikeblock:update(dt, player)

	self:los(player)

	self:checkColTiles()

	self:movement(dt, player)
	if next(self.collisions) ~= nil then
		self.moving = false;
	end

	if self:checkColSpecific(player) then
		player:dealDmg(self.attack)
	end

end

function Spikeblock:movement(dt, player)
	if self.moving == true then
		self.pos.y = self.pos.y + self.speed.y * (dt * 10);
		self.pos.x = self.pos.x + self.speed.x * (dt * 10);
	else
		self.speed = {x = 0, y = 0} 
	end

	if self.pos.y + self.height < 0 or self.pos.y > TCE.GetViewportHeight() then
		self.updateOn = false;
		self.renderOn = false;
	end 

	if self.pos.x + self.width < 0 or self.pos.x > TCE.GetViewportWidth() then
		self.updateOn = false;
		self.renderOn = false;
	end

	self.lastVelocity.x = self.speed.x
	self.lastVelocity.y = self.speed.y
end

function Spikeblock:los(player)
	if self.moving == false then
		if player.pos.x + player.width > self.pos.x and player.pos.x < self.pos.x or player.pos.x < self.pos.x + self.width and player.pos.x > self.pos.x then
			if player.pos.y < self.pos.y then
				self.speed.y = -10;
				self.moving = true;
			else
				self.speed.y = 10;
				self.moving = true;
			end
		self.speed.x = 0;
		elseif player.pos.y + player.height > self.pos.y and player.pos.y < self.pos.y or player.pos.y < self.pos.y + self.height and player.pos.y > self.pos.y then
			if player.pos.x < self.pos.x then
				self.speed.x = -10
				self.moving = true;
			else
				self.speed.x = 10
				self.moving = true;
			end
		self.speed.y = 0;
		end
	end
end
--[[
NPC script contains the code for creating different NPCs, these
consist of the different monsters, projectiles, and power-ups.
All of these can be instantiated from this class and then used
by a different script.
--]]

require("source.utility")
require("source.constant")
require("source.player")

-- the base familiar class
Familiar = {}
Familiar.__index = Familiar

-- base information for familiar classes
Familiar.spritewidth = 32; Familiar.spriteheight = 48
Familiar.spritesheet = graphics.newImageSheet(
	"assets/images/npc_familiar.png",
	{ width = Familiar.spritewidth, height = Familiar.spriteheight, numFrames = 2, sheetContentWidth = 64, sheetContentHeight = 48 }
)
Familiar.animation = { name = "walk", start = 1, count = 2, time = 1000, loopCount = 0, loopDirection = "forward" }
Familiar.sound = audio.loadSound("assets/sounds/hurt.mp3")

-- allows for construction via Familiar() instead of Familiar.new()
setmetatable(Familiar, { __call = function (cls, ...) return cls.new(...) end })

function Familiar.new (_x, _y)
	local self = setmetatable({}, Familiar)

	if Familiar.sound == nil then Familiar.sound = audio.loadSound("assets/sounds/hurt.mp3") end

	self.x = _x; self.y = _y
	self.angle = 0.0
	self.distance = 0
	self.health = Constant.NPC_FAMILIAR_HEALTH
	self.sprite = display.newSprite(Familiar.spritesheet, Familiar.animation)
	self.sprite.y = Constant.CONTENT_CENTER_Y
	self.sprite.isVisible = true
	self.sprite:setSequence("walk")
	self.sprite:play()

	return self
end

function Familiar:enterFrame (_event)
	if self.health > 0 then
		self.distance = Utility.length(self.x, self.y, Player.x, Player.y)

		local deltax = 0; local deltay = 0

		if     Player.x <= self.x and Player.y > self.y then deltax = math.abs(self.x - Player.x); deltay = math.abs(self.y - Player.y)
		elseif Player.x > self.x and Player.y > self.y then deltax = self.x - Player.x; deltay = math.abs(self.y - Player.y)
		elseif Player.x <= self.x and Player.y <= self.y then deltax = math.abs(Player.x - self.x); deltay = Player.y - self.y
		elseif Player.x > self.x and Player.y <= self.y then deltax = self.x - Player.x; deltay = Player.y - self.y end

		self.angle = math.deg(math.atan2(deltay, deltax))
		self.angle = Utility.capAngle(self.angle)

		local tempx = Player.x - self.x; local tempy = Player.y - self.y
		local length = math.sqrt(tempx * tempx + tempy * tempy)
		local unitx = tempx / length; local unity = tempy / length

		self.x = self.x + unitx * Constant.NPC_FAMILIAR_MOVE_SPEED
		self.y = self.y + unity * Constant.NPC_FAMILIAR_MOVE_SPEED
	end
end
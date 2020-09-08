--[[
Weapon script contains the basis for making a weapon "class". This class
contains the neccessary membes and event-driven handler methods for a
weapon to be created and used. The gun is locked to the bottom-center of
the screen and can be fired to kill enemies.
--]]

require("source.constant")

-- the base weapon class
Weapon = {}
Weapon.__index = Weapon

-- allows for construction via Weapon() instead of Weapon.new()
setmetatable(Weapon, { __call = function (cls, ...) return cls.new(...) end })

-- constructor for the weapon class
function Weapon.new ()
	local self = setmetatable({}, Weapon)

	self.locked = false
	self.spritewidth = 48; self.spriteheight = 96
	self.firetime = 500
	self.sound = audio.loadSound("assets/sounds/shot.mp3", {  })
	self.spritesheet = graphics.newImageSheet(
		"assets/images/weapon.png",
		{ width = self.spritewidth, height = self.spriteheight, numFrames = 6, sheetContentWidth = 288, sheetContentHeight = 96 }
	)
	self.animations = {
		{ name = "idle", start = 1, count = 1, time = 0, loopCount = 0, loopDirection = "forward" },
		{ name = "fire", start = 2, count = 5, time = self.firetime, loopCount = 1, loopDirection = "forward" }
	}
	self.sprite = display.newSprite(self.spritesheet, self.animations)
	self.sprite.x = Constant.CONTENT_CENTER_X; self.sprite.y = Constant.CONTENT_HEIGHT - (self.spriteheight / 2)
	self.sprite:setSequence("idle")
	self.sprite:play()

	return self
end

-- fire's the weapon if called, and the weapon is not locked
function Weapon:fire ()
	-- reset the state of the weapon after firing
	local function reset ()
		self.sprite:setSequence("idle")
		self.locked = false
	end

	if not self.locked then
		audio.play(self.sound); audio.play(self.sound) -- probably not the best idea
		self.sprite:setSequence("fire")
		self.sprite:play()
		timer.performWithDelay(self.firetime, reset)
		self.locked = true
	end
end
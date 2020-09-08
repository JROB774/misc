--[[
Weapon script contains the basis for making a weapon "class". This class
contains the neccessary membes and event-driven handler methods for a
weapon to be created and used. The gun is locked to the bottom-center of
the screen and can be fired to kill enemies.
--]]

-- includes functionality from other scripts
require("source.constant")

-- the base weapon class is created
Weapon = {}
Weapon.__index = Weapon

-- allows for construction via Weapon() instead of Weapon.new()
setmetatable(Weapon, { __call = function (cls, ...) return cls.new(...) end })

--[[
This function is used to create a new instance of the Weapon class
at the specified position. It can be invoked either through Weapon()
or Weapon.new().
--]]

function Weapon.new ()
	-- creates a new weapon instance to be initialised
	local self = setmetatable({}, Weapon)

	-- sets the initial values of the weapon (images, sprites/animations, sound, etc.)
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

	-- returns the new instance for use
	return self
end

--[[
The fire function is to be invoked whenever the gun is to fire, usually
after a tap, key-press, or other type of input. It will play the neccessary
sounds and animations, and will not be fired again until after cooldown.
--]]

function Weapon:fire ()
	--[[
	The reset function is a callback that is triggered after a certain
	amount of time elapses, post-firing. It unlocks the gun and returns
	it to its idle state, so that it can be fired again.
	--]]

	local function reset ()
		self.sprite:setSequence("idle")
		self.locked = false
	end

	-- if the gun is not locked/cooling down then the gun can be fired
	if not self.locked then
		-- layers the shot sound to make it louder
		audio.play(self.sound); audio.play(self.sound)
		-- plays the weapon's firing animation
		self.sprite:setSequence("fire")
		self.sprite:play()
		-- once the animation is complete, the weapon is unlocked
		timer.performWithDelay(self.firetime, reset)
		self.locked = true
	end
end
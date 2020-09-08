--[[
NPC script contains the code for creating different NPCs, these consist
of the different monsters. All of these can be instantiated from this
class and then used by a different script.
--]]

-- includes functionality from other scripts
require("source.utility")
require("source.constant")
require("source.player")

-- the base familiar class is created
Familiar = {}
Familiar.__index = Familiar

-- base infromation shared across all instances is set (texture, sprites/animations, sound, etc.)
Familiar.spritewidth = 32; Familiar.spriteheight = 48
Familiar.spritesheet = graphics.newImageSheet(
	"assets/images/npc_familiar.png",
	{ width = Familiar.spritewidth, height = Familiar.spriteheight, numFrames = 2, sheetContentWidth = 64, sheetContentHeight = 48 }
)
Familiar.animation = { name = "walk", start = 1, count = 2, time = 1000, loopCount = 0, loopDirection = "forward" }
Familiar.sound = audio.loadSound("assets/sounds/hurt.mp3")

-- allows for construction via Familiar() instead of Familiar.new()
setmetatable(Familiar, { __call = function (cls, ...) return cls.new(...) end })

--[[
This function is used to create a new instance of the Familiar class
at the specified position. It can be invoked either through Familiar()
or Familiar.new(). It does not setup the neccessary event listeners for
the NPC, so they must be set by the function caller.
--]]

function Familiar.new (_x, _y)
	-- creates a new familiar instance to be initialised
	local self = setmetatable({}, Familiar)

	-- if the global familiar sound hasn't been created, then create it
	if Familiar.sound == nil then Familiar.sound = audio.loadSound("assets/sounds/hurt.mp3") end

	-- sets the initial values of the familiar NPC
	self.x = _x; self.y = _y
	self.angle = 0.0
	self.distance = 0
	self.health = Constant.NPC_FAMILIAR_HEALTH
	self.sprite = display.newSprite(Familiar.spritesheet, Familiar.animation)
	self.sprite.y = Constant.CONTENT_CENTER_Y
	self.sprite.isVisible = true
	self.sprite:setSequence("walk")
	self.sprite:play()

	-- returns the new instance for use
	return self
end

--[[
This function is the event handler for the Familiar class, which is
called every frame to update the NPC. Whilst the Familiar is alive
it will constantly move towards the player at a constant speed. The
event listener is not setup by default, so the creator of the
instance must add one.
--]]

function Familiar:enterFrame (_event)
	-- only updates the familiar if it is still alive
	if self.health > 0 then
		-- calculates the distance between itself and the player
		self.distance = Utility.length(self.x, self.y, Player.x, Player.y)

		-- determines the difference between the position of the player and itself
		local deltax = 0; local deltay = 0

		if     Player.x <= self.x and Player.y > self.y then deltax = math.abs(self.x - Player.x); deltay = math.abs(self.y - Player.y)
		elseif Player.x > self.x and Player.y > self.y then deltax = self.x - Player.x; deltay = math.abs(self.y - Player.y)
		elseif Player.x <= self.x and Player.y <= self.y then deltax = math.abs(Player.x - self.x); deltay = Player.y - self.y
		elseif Player.x > self.x and Player.y <= self.y then deltax = self.x - Player.x; deltay = Player.y - self.y end

		-- uses this delta to calculate what angle it should face to move towards the player
		self.angle = math.deg(math.atan2(deltay, deltax))
		self.angle = Utility.capAngle(self.angle)

		-- determines its new X and Y position based on this angle, and how far it should move
		local tempx = Player.x - self.x; local tempy = Player.y - self.y
		local length = math.sqrt(tempx * tempx + tempy * tempy)
		local unitx = tempx / length; local unity = tempy / length

		self.x = self.x + unitx * Constant.NPC_FAMILIAR_MOVE_SPEED
		self.y = self.y + unity * Constant.NPC_FAMILIAR_MOVE_SPEED
	end
end
local TCE = require("data.scripts.tce.tce")

Player = setmetatable({}, { __index = Entity })
Player.__index = Player
function Player.new (pos)
	local self = setmetatable(
		Entity.new(
			pos, 20, 20, 0,
			{health = 30, 
			  attack = 3, 
			  velocity = {x = 10, y = 10},
			  lastVelocity = {x = 0, y = 0},
			  radius = 60}),
			{solid = true, 
			  renderOn = true, 
			  updateOn = true, 
			  tag = "player",
			  state = ""},
			tex["player"]),
		Defender)

	self.stats.accelerationStep = 1.5
	self.stats.decelerationStep = 1.5
	self.stats.acceleration = self.stats.accelerationStep * self.width -- self.width used to be TILE_WIDTH here.
	self.stats.deceleration = self.stats.decelerationStep * self.width
	self.stats.maxSpeed = 5.00 * self.width
	self.light = TCE.AddLight(self.pos.x+(self.width/2), self.pos.y+(self.height/2), 128.0, 8.0)

	return self

	table.insert(renderList, self.id, self)

	return self
end

function Player:update (dt)
	-- Update the player's position and check for collisions with tiles/entities.
	self:movement(dt)

	self.collisions = TCE.checkColWithList(self, tileList)
	for key in pairs(self.collisions) do
		self:movementRebound(self.collision[key])
	end
	self.collisions = TCE.checkColWithList(self, entityList)
	for key in pairs(self.collisions) do
		self:movementRebound(self.collision[key])
	end
	self.collisions = {}

	-- @TEMP: WE WON'T NEED ENTITY TRACKING IN THE END THIS IS JUST FOR DEBUG
	TCE.CameraTrack(player)

	TCE.UpdateLight(self.light, self.pos.x+(self.width/2), self.pos.y+(self.height/2), 128.0, 8.0)

	-- If the left mouse button is pressed then we want to shoot a love heart.
	if (TCE.ButtonPressed(TCE.Buttons.BUTTON_LEFT)) then
		self:shootHeartCannon()
	end
end

function  Player:shootHeartCannon ()
	local mousePos = { x = 0, y = 0 }
	mousePos.x, mousePos.y = TCE.GetMousePosition()

	Heart.new(self.pos, mousePos)
end

function Player:movement (dt)
	self.stats.velocity.x = 0.0
	self.stats.velocity.y = 0.0

	if TCE.IsKeyDown(TCE.Keycodes.KEY_W) then self.stats.velocity.y = self.stats.velocity.y - self.stats.maxSpeed end
	if TCE.IsKeyDown(TCE.Keycodes.KEY_S) then self.stats.velocity.y = self.stats.velocity.y + self.stats.maxSpeed end
	if TCE.IsKeyDown(TCE.Keycodes.KEY_A) then self.stats.velocity.x = self.stats.velocity.x - self.stats.maxSpeed end
	if TCE.IsKeyDown(TCE.Keycodes.KEY_D) then self.stats.velocity.x = self.stats.velocity.x + self.stats.maxSpeed end

	-- We want to round this so that the player will not jitter with collision!
	self.pos.y = TCE.Round(self.pos.y + (self.velocity.y * dt))
	self.pos.x = TCE.Round(self.pos.x + (self.velocity.x * dt))
end

function Player:render ()
	-- We want to center the player's texture on the physical collision box of the player.
	local renderX = self.pos.x - ((TCE.GetAnimationFrameWidth(anm["player_walk"]) - self.width) / 2)
	local renderY = self.pos.y - ((TCE.GetAnimationFrameHeight(anm["player_walk"]) - self.height) / 2)

	local transform = { x = renderX, y = renderY, angle = self.angle }

	-- Determine whether to the flip the player sprite based on the mouse's position.
	local mouse = { x = 0, y = 0 }
	mouse.x, mouse.y = TCE.GetMousePosition()
	if mouse.x < (self.pos.x + (self.width / 2)) then transform.flip = TCE.RenderFlips.HORZ
	else transform.flip = TCE.RenderFlips.NONE end

	-- Based on whether we are moving or not we want to play the walking or idle animation.
	if self.stats.velocity.x ~= 0 or self.stats.velocity.y ~= 0 then
		TCE.RenderAnimatedTexture(self.texture, anm["player_walk"], transform)
	else
		TCE.RenderAnimatedTexture(self.texture, anm["player_idle"], transform)
	end
end
local TCE = require("data.scripts.tce.tce")

Player = setmetatable({}, { __index = Entity })
Player.__index = Player

function Player.new (x, y, health, attack, speed, angle)
	local self = setmetatable(
		Entity.new(x, y, TILE_WIDTH, TILE_HEIGHT, health, attack, speed, angle, true, true, true, tex["player"], "player"),
		Player)

	self.velocity          = { x = 0.0, y = 0.0 }
	self.acceleration_step = 1.5
	self.deceleration_step = 1.5
	self.acceleration      = self.acceleration_step * TILE_WIDTH
	self.deceleration      = self.deceleration_step * TILE_WIDTH
	self.max_speed         = 5.00 * TILE_WIDTH
	self.light             = TCE.AddLight(self.pos.x+(self.width/2), self.pos.y+(self.height/2), 128.0, 8.0)

	return self
end

function Player:update (dt)
	-- Update the player's position and check for collisions with tiles/entities.
	self:movement(dt)
	self:checkBounds()
	self:checkColTiles()

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

	HeartBullet.new(self.pos.x, self.pos.y, 2, 500, mousePos, self)
end

--[[
function Player:handleVelocityDirection (key_1, key_2, velocity)
	-- Accelerate the velocity if a directional key is being pressed.
	if TCE.IsKeyDown(key_1) then velocity = velocity - self.acceleration
	elseif TCE.IsKeyDown(key_2) then velocity = velocity + self.acceleration
	else
		-- If the player is not moving then decelerate, if necessary.
		if velocity > self.deceleration_step then velocity = velocity - self.deceleration
		elseif velocity < -self.deceleration_step then velocity = velocity + self.deceleration
		else velocity = 0.0 end
	end

	-- Bounds check the velocity so the player does not exceed max speed.
	if velocity < -self.max_speed then velocity = -self.max_speed
	elseif velocity > self.max_speed then velocity = self.max_speed end

	return velocity
end
--]]

function Player:movement (dt)
	-- Update both the vertical and horizontal velocity for the player.
	-- self.velocity.y = self:handleVelocityDirection(TCE.Keycodes.KEY_W, TCE.Keycodes.KEY_S, self.velocity.y)
	-- self.velocity.x = self:handleVelocityDirection(TCE.Keycodes.KEY_A, TCE.Keycodes.KEY_D, self.velocity.x)

	self.velocity.x = 0.0
	self.velocity.y = 0.0

	if TCE.IsKeyDown(TCE.Keycodes.KEY_W) then self.velocity.y = self.velocity.y - self.max_speed end
	if TCE.IsKeyDown(TCE.Keycodes.KEY_S) then self.velocity.y = self.velocity.y + self.max_speed end
	if TCE.IsKeyDown(TCE.Keycodes.KEY_A) then self.velocity.x = self.velocity.x - self.max_speed end
	if TCE.IsKeyDown(TCE.Keycodes.KEY_D) then self.velocity.x = self.velocity.x + self.max_speed end

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
	if self.velocity.x ~= 0 or self.velocity.y ~= 0 then
		TCE.RenderAnimatedTexture(self.texture, anm["player_walk"], transform)
	else
		TCE.RenderAnimatedTexture(self.texture, anm["player_idle"], transform)
	end
end

function Player:checkBounds ()
	--[[
	if self.pos.x < 0 then
		Map.worldPosition.x = Map.worldPosition.x - 1
		self.pos.x = TCE.GetViewportWidth()
	elseif self.pos.x > TCE.GetViewportWidth() then
		Map.worldPosition.x = Map.worldPosition.x + 1
		self.pos.x = 0
	elseif self.pos.y < 0 then
		Map.worldPosition.y = Map.worldPosition.y - 1
		self.pos.y = TCE.GetViewportHeight()
	elseif self.pos.y > TCE.GetViewportHeight() then
		Map.worldPosition.y = Map.worldPosition.y + 1
		self.pos.y = 0
	end

	file = TCE.GetRoomFromWorldPosition(Map.worldPosition.x, Map.worldPosition.y)
	if currentRoom ~= file then
		currentRoom = file
		resetLists()
		Map.loadRoom(file)
	end
	--]]
end

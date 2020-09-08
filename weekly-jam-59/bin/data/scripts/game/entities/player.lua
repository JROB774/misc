local TCE = require("data.scripts.tce.tce")

Player = setmetatable({}, { __index = Entity })
Player.__index = Player
function Player.new (pos)
	local self = setmetatable(
		Entity.new(
			pos, 9, 16, 0,
			{health = 6,
				maxHealth = 6, -- Health starts full
				attack = 3,
				velocity = {x = 10, y = 10},
				maxSpeed = 5.00 * TILE_WIDTH, -- Refer to speed in terms of tiles per second.
				lastVelocity = {x = 0, y = 0},
				stunDelay = {timer = 0, max = 2}},
			{solid = true,
			  	renderOn = true,
			  	updateOn = true,
			  	tag = "player",
			  	state = "",
			  	items = {}},
			tex["player"],
			20),
		Player)

	self.zIndex = Z_INDEX.PLAYER
	self.light = TCE.AddLight(self.pos.x+(self.width/2), self.pos.y+(self.height/2), 128.0, 8.0)

	self.pos.x = self.pos.x - cameraX
	self.pos.y = self.pos.y - cameraY

	self:uniqueIdAssign()

	renderList[self.id] = self
	return self
end

function Player:update (dt)
	-- Update the player's position and check for collisions with tiles/entities.
	self:movement(dt)

	self:stunTimer(dt)

	self.collisions = checkColWithList(self, tileList)
	for key in pairs(self.collisions) do
		self:movementRebound(self.collisions[key])
	end



	if self.flags.state ~= "stunned" then
		--[[self.collisions = checkColWithList(self, entityList)
		for key in pairs(self.collisions) do
			if self.collisions[key].flags.tag ~= "heart" and
				self.collisions[key].flags.tag ~= "player" and
				self.collisions[key].flags.tag ~= "pickUp" then
				--self:movementRebound(self.collisions[key])
			end
		end
		self.collisions = {}
		]]--
		-- If the left mouse button is pressed then we want to shoot a love heart.
		if (TCE.ButtonPressed(TCE.Buttons.BUTTON_LEFT)) then
			self:shootHeartCannon()
		end

	end

	-- @TEMP:
	TCE.UpdateLight(self.light, self.pos.x+(self.width/2), self.pos.y+(self.height/2), 128.0, 8.0)
end

function Player:render ()
	-- We want to center the player's texture on the physical collision box of the player.
	local renderX = self.pos.x - ((TCE.GetAnimationFrameWidth(anm["player_walk"]) - self.width) / 2)
	local renderY = self.pos.y - ((TCE.GetAnimationFrameHeight(anm["player_walk"]) - self.height) / 2)

	local transform = { x = renderX, y = renderY, angle = self.angle }

	-- Determine whether to the flip the player sprite based on the mouse's position.
	local mousex, mousey = TCE.GetMousePosition()
	if mousex < (self.pos.x + (self.width / 2)) then transform.flip = TCE.RenderFlips.HORZ
	else transform.flip = TCE.RenderFlips.NONE end

	-- Based on whether we are moving or not we want to play the walking or idle animation.
	if self.stats.velocity.x ~= 0 or self.stats.velocity.y ~= 0 then
		TCE.RenderAnimatedTexture(self.texture, anm["player_walk"], transform)
	else
		TCE.RenderAnimatedTexture(self.texture, anm["player_idle"], transform)
	end
end

function Player:shootHeartCannon ()
	-- Bit of a hack cos it interacts with character

	-- Let us assume circle.
	local mousePos = { x = 0, y = 0 }
	mousePos.x, mousePos.y = TCE.GetMousePosition()
	local dir = TCE.NormalizeVector(TCE.LookAtVector(mousePos, self.pos))
	local startPos = {
		x = (self.pos.x + self.width/2 + dir.x * self.width)/TILE_WIDTH,
		y = (self.pos.y + 5 + dir.y * self.height)/TILE_HEIGHT }
	print(startPos.x)
	Heart.new(startPos, mousePos)
end

function Player:movement (dt)
	self.stats.velocity.x = 0.0
	self.stats.velocity.y = 0.0

	if TCE.IsKeyDown(TCE.Keycodes.KEY_W) then self.stats.velocity.y = self.stats.velocity.y - self.stats.maxSpeed end
	if TCE.IsKeyDown(TCE.Keycodes.KEY_S) then self.stats.velocity.y = self.stats.velocity.y + self.stats.maxSpeed end
	if TCE.IsKeyDown(TCE.Keycodes.KEY_A) then self.stats.velocity.x = self.stats.velocity.x - self.stats.maxSpeed end
	if TCE.IsKeyDown(TCE.Keycodes.KEY_D) then self.stats.velocity.x = self.stats.velocity.x + self.stats.maxSpeed end

	-- We want to round this so that the player will not jitter with collision!
	self.pos.y = TCE.Round(self.pos.y + (self.stats.velocity.y * dt))
	self.pos.x = TCE.Round(self.pos.x + (self.stats.velocity.x * dt))
end

function Player:updateItems(itemType)
	itemHave = false
	if itemType == "healthSmall" then
		self:dealDmg(-5)
		return
	end

	for key in pairs(self.flags.items) do
		if self.flags.items[key] == itemType then
			itemHave = true
			return
		end
	end

	if itemHave then
		table.insert(itemType, self.flags.items)
		if itemType == "speed" then
			self.stats.maxSpeed = self.stats.maxSpeed + 10 * TILE_WIDTH
		elseif itemType == "power" then
			self.stats.attack = self.stats.attack + 5
		end
 	end
end

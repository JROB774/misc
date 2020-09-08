--[[
Player script contains the basis for the player. This namespace
contains the neccessary members and event-driven handler methods for a
player to be created and run within the game.
--]]

-- includes functionality from other scripts
require("source.utility")
require("source.constant")
require("source.collision")
require("source.raycast")
require("source.map")
require("source.weapon")
require("source.npclist")

-- the global player namespace is created
Player = {}
Player.__index = Player

--[[
This function spawns the player into the game, taking in a starting
position and angle. It also initialises all of the player's info, such
as images, sounds, keys, score, controls, etc. which will be used
throughout the player's lifetime.
--]]

function Player.spawn (_x, _y, _angle)
	-- sets the initial values of the player on spawn
	Player.x = _x; Player.y = _y
	Player.angle = _angle
	Player.dead = false
	Player.sound = audio.loadSound("assets/sounds/death.mp3")
	Player.score = 0
	Player.keys = {}
	Player.keys.up = false; Player.keys.right = false; Player.keys.down = false; Player.keys.left = false
	Player.keys.turnleft = false; Player.keys.turnright = false
	Player.controls = {}
	Player.controls.move = display.newImageRect("assets/images/control_move.png", 76, 76)
	Player.controls.move.x = 47; Player.controls.move.y = 197
	Player.controls.move.bounds = {}
	Player.controls.move.bounds.all = { x = 10, y = 160, w = 76, h = 76 }
	Player.controls.move.bounds.up = { x = 30, y = 160, w = 36, h = 38 }
	Player.controls.move.bounds.right = { x = 48, y = 180, w = 38, h = 36 }
	Player.controls.move.bounds.down = { x = 30, y = 198, w = 36, h = 38 }
	Player.controls.move.bounds.left = { x = 10, y = 180, w = 38, h = 36 }
	Player.controls.move.keys = {}
	Player.controls.move.keys.up = false
	Player.controls.move.keys.right = false
	Player.controls.move.keys.down = false
	Player.controls.move.keys.left = false
	Player.controls.aim = display.newImageRect("assets/images/control_aim.png", 76, 56)
	Player.controls.aim.x = 271; Player.controls.aim.y = 198
	Player.controls.aim.bounds = {}
	Player.controls.aim.bounds.all = { x = 234, y = 171, w = 76, h = 56 }
	Player.controls.aim.bounds.left = { x = 234, y = 192, w = 38, h = 35 }
	Player.controls.aim.bounds.leftfire = { x = 240, y = 177, w = 24, h = 24 }
	Player.controls.aim.bounds.fire = { x = 255, y = 171, w = 34, h = 38 }
	Player.controls.aim.bounds.rightfire = { x = 280, y = 177, w = 24, h = 24 }
	Player.controls.aim.bounds.right = { x = 272, y = 192, w = 38, h = 35 }
	Player.controls.aim.keys = {}
	Player.controls.aim.keys.left = false
	Player.controls.aim.keys.leftfire = false
	Player.controls.aim.keys.fire = false
	Player.controls.aim.keys.rightfire = false
	Player.controls.aim.keys.right = false
	Player.weapon = Weapon()
end

--[[
This function is an event handler triggered whenever a key is pressed/released.
It is designed to store debug controls so that the game can be played on a PC
(as the touch controls would not be usable). This function should not be used
in the final game.

The event listener for this function is not set automatically, so it is down
to the caller to add the listener.
--]]

function Player:key (_event)
	if _event.phase == "down" then
		-- if the key was pressed then the appropriate key flag/state should be enabled
		if     _event.keyName == "w"      then Player.controls.move.keys.up = true
		elseif _event.keyName == "d"      then Player.controls.move.keys.right = true
		elseif _event.keyName == "s"      then Player.controls.move.keys.down = true
		elseif _event.keyName == "a"      then Player.controls.move.keys.left = true
		elseif _event.keyName == "left"   then Player.controls.aim.keys.left = true
		elseif _event.keyName == "right"  then Player.controls.aim.keys.right = true
		elseif _event.keyName == "space"  then Player.controls.aim.keys.fire = true end
	elseif _event.phase == "up" then
		-- if the key was released then the appropriate key flag/state should be disabled
		if     _event.keyName == "w"      then Player.controls.move.keys.up = false
		elseif _event.keyName == "d"      then Player.controls.move.keys.right = false
		elseif _event.keyName == "s"      then Player.controls.move.keys.down = false
		elseif _event.keyName == "a"      then Player.controls.move.keys.left = false
		elseif _event.keyName == "left"   then Player.controls.aim.keys.left = false
		elseif _event.keyName == "right"  then Player.controls.aim.keys.right = false
		elseif _event.keyName == "space"  then Player.controls.aim.keys.fire = false end
	end

	-- overrides default OS behaviour (could cause issues)
	return true
end

--[[
This event handler is used to handle the actual controls of the game. All of
these controls are managed through an on-screen virtual controller. Touching
specific elements of the controller will set certain key flags, these flags
are checked later in the code and if set they will run the appropriate code.

The event listener for this function is not set automatically, so it is down
to the caller to add the listener.
--]]

function Player:touch (_event)
	-- gets the position where the tap event occurred on screen
	pos = { x = _event.x, y = _event.y }

	-- if the touch event is over...
	if _event.phase == "ended" or _event.phase == "cancelled" then
		-- reset all move keys if the touch ended over the move controls
		if Collision.pointAndQuad(pos, Player.controls.move.bounds.all) then
			Player.controls.move.keys.up = false
			Player.controls.move.keys.right = false
			Player.controls.move.keys.down = false
			Player.controls.move.keys.left = false
		end
		-- reset all aim keys if the touch ended over the aim controls
		if Collision.pointAndQuad(pos, Player.controls.aim.bounds.all) then
			Player.controls.aim.keys.left = false
			Player.controls.aim.keys.leftfire = false
			Player.controls.aim.keys.fire = false
			Player.controls.aim.keys.rightfire = false
			Player.controls.aim.keys.right = false
		end

		-- stops the touch carrying on to lower objects 
		return true
	end

	-- based on where the user's finger is, activate/deactivate the corresponding move keys
	if Collision.pointAndQuad(pos, Player.controls.move.bounds.up) then Player.controls.move.keys.up = true
	else Player.controls.move.keys.up = false end
	if Collision.pointAndQuad(pos, Player.controls.move.bounds.right) then Player.controls.move.keys.right = true
	else Player.controls.move.keys.right = false end
	if Collision.pointAndQuad(pos, Player.controls.move.bounds.down) then Player.controls.move.keys.down = true
	else Player.controls.move.keys.down = false end
	if Collision.pointAndQuad(pos, Player.controls.move.bounds.left) then Player.controls.move.keys.left = true
	else Player.controls.move.keys.left = false end

	-- based on where the user's finger is, activate/deactivate the corresponding aim keys
	if Collision.pointAndQuad(pos, Player.controls.aim.bounds.left) then Player.controls.aim.keys.left = true
	else Player.controls.aim.keys.left = false end
	if Collision.pointAndQuad(pos, Player.controls.aim.bounds.leftfire) then Player.controls.aim.keys.leftfire = true
	else Player.controls.aim.keys.leftfire = false end
	if Collision.pointAndQuad(pos, Player.controls.aim.bounds.fire) then Player.controls.aim.keys.fire = true
	else Player.controls.aim.keys.fire = false end
	if Collision.pointAndQuad(pos, Player.controls.aim.bounds.rightfire) then Player.controls.aim.keys.rightfire = true
	else Player.controls.aim.keys.rightfire = false end
	if Collision.pointAndQuad(pos, Player.controls.aim.bounds.right) then Player.controls.aim.keys.right = true
	else Player.controls.aim.keys.right = false end

	-- stops the touch carrying on to lower objects
	return true
end

--[[
This event handler is called every single frame, it updates the player and
handles raycasting. The player's controls are checked and appropriate
action is taken (player is moved, rotated, weapon is fired, etc.) Collision
is also checked with the map to make sure the player does not pass walls.
After the player has been updated the function uses the player's position
and angle to cast the rays which will be used by the map to render the scene.

The event listener for this function is not set automatically, so it is down
to the caller to add the listener.
--]]

function Player:enterFrame (_event)
	-- if any of the aiming/turn keys are being pressed then the player's angle is turned accordingly
	if Player.controls.aim.keys.left or Player.controls.aim.keys.leftfire then Player.angle = Player.angle + Constant.PLAYER_TURN_SPEED
	elseif Player.controls.aim.keys.right or Player.controls.aim.keys.rightfire then Player.angle = Player.angle - Constant.PLAYER_TURN_SPEED end
	Player.angle = Utility.capAngle(Player.angle)

	-- declares values to store the player's displacement
	local dispx = 0
	local dispy = 0

	-- if the up key is pressed then the player should be moved forward relative to the current angle
	if Player.controls.move.keys.up then
		if Player.angle >= 0.0 and Player.angle < 180.0 then
			dispx = dispx + (math.cos(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy - (math.abs(math.sin(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED))
		else
			dispx = dispx + (math.cos(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy + (math.abs(math.sin(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED))
		end
	-- if the down key is pressed then the player should be moved backward relative to the current angle
	elseif Player.controls.move.keys.down then
		if Player.angle >= 0.0 and Player.angle < 180.0 then
			dispx = dispx - (math.cos(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy +  (math.abs(math.sin(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED))
		else
			dispx = dispx - (math.cos(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy - (math.abs(math.sin(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED))
		end
	end

	-- depending on the move key being pressed a temporary angle is used for strafing
	local tempangle
	if Player.controls.move.keys.left then tempangle = Utility.capAngle(Player.angle + 90.0)
	elseif Player.controls.move.keys.right then tempangle = Utility.capAngle(Player.angle - 90.0) end

	-- if the left/right key is pressed then the player should strafe relative to the current angle
	if Player.controls.move.keys.left or Player.controls.move.keys.right then
		if tempangle >= 0.0 and tempangle < 180.0 then
			dispx = dispx + (math.cos(math.rad(tempangle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy - (math.abs(math.sin(math.rad(tempangle)) * Constant.PLAYER_MOVE_SPEED))
		else
			dispx = dispx + (math.cos(math.rad(tempangle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy + (math.abs(math.sin(math.rad(tempangle)) * Constant.PLAYER_MOVE_SPEED))
		end
	end

	-- if any of the keys involving the firing of the weapon occurrs then fire
	if Player.controls.aim.keys.leftfire or Player.controls.aim.keys.rightfire or Player.controls.aim.keys.fire then
		-- check to see if the weapon is locked, if so then do not check for hits
		if not Player.weapon.locked then
			-- go through each living enemy do determine if a hit occurred
			for i = 1, Npclist.count do
				if Npclist.npcs[i].health > 0 then
					-- if the player's angle is the same as the NPCs angle (with some added easing due to width) then its a hit
					if Player.angle >= (Npclist.npcs[i].angle - Constant.AIM_EASING) and Player.angle <= (Npclist.npcs[i].angle + Constant.AIM_EASING) then
						-- the enemy is hurt with health deducted and a noise played
						Npclist.npcs[i].health = Npclist.npcs[i].health - 1
						audio.play(Familiar.sound)
						-- if the enemy was only hurt then five points are earned
						if Npclist.npcs[i].health > 0 then Player.score = Player.score + Constant.SCORE_HIT
						else
							-- if the enemy died thn it is no longer visible and the player gains ten points
							Npclist.npcs[i].sprite.isVisible = false
							Player.score = Player.score + Constant.SCORE_KILL
						end
					end
				end
			end
		end

		-- the weapon is fired (only if it is not still locked)
		Player.weapon:fire()
	end

	-- checks the player's soon-to-be new position for collisions
	local collided = false
	local pos = { x = Player.x + dispx, y = Player.y + dispy }

	-- goes through each map tile to check if the player is going to collide with it
	for iy = 1, Map.height do
		for ix = 1, Map.width do
			-- makes sure the tile is solid before checking collision (otherwise player would always be stuck)
			if Map[iy][ix] == Constant.TILE_SOLID then
				-- if the player collided with the tile then it will not be moved to the new position (very basic/bad temp collision)
				local tile = { x = (ix - 1) * Constant.TILE_WIDTH, y = (iy - 1) * Constant.TILE_WIDTH, w = Constant.TILE_WIDTH, h = Constant.TILE_WIDTH }
				if Collision.pointAndQuad(pos, tile) then
					dispx = 0; dispy = 0
					collided = true
					break
				end
			end
		end
		-- once a collision occurrs save CPU time by breaking from the loop
		if collided then break end
	end

	-- add the displacement if no collisions occurred
	Player.x = Player.x + dispx
	Player.y = Player.y + dispy

	-- selects the angle to start casting by adding half the FOV to the current angle
	local rayangle = Player.angle + (Constant.PLAYER_FOV / 2)

	-- goes through and casts rays for each slice of the screen
	for i = 1, Constant.PROJPLANE_WIDTH do
		-- caps the rayangle to ensure incase it went out of bounds
		rayangle = Utility.capAngle(rayangle)

		-- used to see if a ray is too long
		local horzmax = false
		local vertmax = false

		-- determines the vertical and horizontal direction of the current ray
		local horzdir;
		if rayangle >= 90.0 and rayangle < 270.0 then horzdir = Utility.dir.LEFT
		else horzdir = Utility.dir.RIGHT end
		local vertdir;
		if rayangle >= 0.0 and rayangle < 180.0 then vertdir = Utility.dir.UP
		else vertdir = Utility.dir.DOWN end

		-- calculates the first horizontal tile-intersection point
		local horzpos = { x = 0, y = 0 };
		if vertdir == Utility.dir.UP then horzpos.y = math.floor(Player.y / Constant.TILE_WIDTH) * (Constant.TILE_WIDTH) - 1
		else horzpos.y = math.floor(Player.y / Constant.TILE_WIDTH) * (Constant.TILE_WIDTH) + Constant.TILE_WIDTH end
		horzpos.x = Player.x + (Player.y - horzpos.y) / math.tan(math.rad(rayangle))

		-- if there is a wall at this point then the ray no longer needs to be cast, and it will stop
		local horztile = { x = math.floor(horzpos.x / Constant.TILE_WIDTH) + 1, y = math.floor(horzpos.y / Constant.TILE_WIDTH) + 1 }
		if Utility.inBounds(horztile.x, 1, Map.width) and Utility.inBounds(horztile.y, 1, Map.height) then
			if Map[horztile.y][horztile.x] == Constant.TILE_EMPTY then
				-- however, if not we want to keep going until there is a wall (or the max ray length is reached)
				local horzjump
				local vertjump
				-- calculates how much it has to jump to reach the next horizontal intersection point
				if vertdir == Utility.dir.UP then
					vertjump = -Constant.TILE_WIDTH
					horzjump = Constant.TILE_WIDTH / math.tan(math.rad(rayangle))
				else
					vertjump = Constant.TILE_WIDTH
					horzjump = -(Constant.TILE_WIDTH / math.tan(math.rad(rayangle)))
				end
				-- keeps casting until a wall is hit or the max ray length is reached
				for i = 1, Constant.RAY_LENGTH do
					if i == Constant.RAY_LENGTH then horzmax = true end
					horzpos.x = horzpos.x + horzjump
					horzpos.y = horzpos.y + vertjump
					horztile.x = math.floor(horzpos.x / Constant.TILE_WIDTH) + 1
					horztile.y = math.floor(horzpos.y / Constant.TILE_WIDTH) + 1
					if Utility.inBounds(horztile.x, 1, Map.width) and Utility.inBounds(horztile.y, 1, Map.height) then
						if Map[horztile.y][horztile.x] == Constant.TILE_SOLID then break end
					end
				end
			end
		end

		-- calculates the first vertical tile-intersection point
		local vertpos = { x, y }
		if horzdir == Utility.dir.LEFT then vertpos.x = math.floor(Player.x / Constant.TILE_WIDTH) * (Constant.TILE_WIDTH) - 1
		else vertpos.x = math.floor(Player.x / Constant.TILE_WIDTH) * (Constant.TILE_WIDTH) + Constant.TILE_WIDTH end
		vertpos.y = Player.y + (Player.x - vertpos.x) * math.tan(math.rad(rayangle))

		-- if there is a wall at this point then the ray no longer needs to be cast, and it will stop
		local verttile = { x = math.floor(vertpos.x / Constant.TILE_WIDTH) + 1, y = math.floor(vertpos.y / Constant.TILE_WIDTH) + 1 }
		if Utility.inBounds(verttile.x, 1, Map.width) and Utility.inBounds(verttile.y, 1, Map.height) then
			if Map[verttile.y][verttile.x] == Constant.TILE_EMPTY then
				-- however, if not we want to keep going until there is a wall (or the max ray length is reached)
				local vertjump
				local horzjump
				-- calculates how much it has to jump to reach the next vertical intersection point
				if horzdir == Utility.dir.LEFT then
					horzjump = -Constant.TILE_WIDTH
					vertjump = Constant.TILE_WIDTH * math.tan(math.rad(rayangle))
				else
					horzjump = Constant.TILE_WIDTH
					vertjump = -(Constant.TILE_WIDTH * math.tan(math.rad(rayangle)))
				end
				-- keeps casting until a wall is hit or the max ray length is reached
				for i = 1, Constant.RAY_LENGTH do
					if i == Constant.RAY_LENGTH then vertmax = true end
					vertpos.x = vertpos.x + horzjump
					vertpos.y = vertpos.y + vertjump
					verttile.x = math.floor(vertpos.x / Constant.TILE_WIDTH) + 1
					verttile.y = math.floor(vertpos.y / Constant.TILE_WIDTH) + 1
					if Utility.inBounds(verttile.x, 1, Map.width) and Utility.inBounds(verttile.y, 1, Map.height) then
						if Map[verttile.y][verttile.x] == Constant.TILE_SOLID then break end
					end
				end
			end
		end

		-- calculates the length of the horizontal and vertical tile-intersection rays
		local horzlen = Utility.length(Player.x, Player.y, horzpos.x, horzpos.y)
		local vertlen = Utility.length(Player.x, Player.y, vertpos.x, vertpos.y)

		-- determines which ray is shorter (this is the ray that should be used for casting)
		if math.abs(horzlen) <= math.abs(vertlen) then
			-- if the ray was out of bounds then it is saved as such, so the renderer knows
			if horzmax then Raycast.rays[i] = Constant.RAY_OUT_OF_BOUNDS
			else
				-- otherwise, the neccessary information for rendering is stored in a table
				Raycast.rays[i] = math.abs(horzlen * math.cos(math.rad(-(Constant.PLAYER_FOV / 2) + (Constant.RAY_ANGLE * (i - 1)))))
				Raycast.offsets[i] = math.floor(horzpos.x % Constant.TILE_WIDTH)
				Raycast.dirs[i] = "horz"
			end
		else
			-- if the ray was out of bounds then it is saved as such, so the renderer knows
			if vertmax then Raycast.rays[i] = Constant.RAY_OUT_OF_BOUNDS
			else
				-- otherwise, the neccessary information for rendering is stored in a table
				Raycast.rays[i] = math.abs(vertlen * math.cos(math.rad(-(Constant.PLAYER_FOV / 2) + (Constant.RAY_ANGLE * (i - 1)))))
				Raycast.offsets[i] = math.floor(vertpos.y % Constant.TILE_WIDTH)
				Raycast.dirs[i] = "vert"
			end
		end

		-- moves on to the next ray until complete
		rayangle = rayangle - Constant.RAY_ANGLE
	end
end
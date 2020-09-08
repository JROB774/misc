--[[
Player script contains the basis for the player. This namespace
contains the neccessary members and event-driven handler methods for a
player to be created and run within the game.
--]]

require("source.utility")
require("source.constant")
require("source.collision")
require("source.raycast")
require("source.map")
require("source.weapon")
require("source.npclist")

-- the global player namespace
Player = {}
Player.__index = Player

-- constructor for the player class
function Player.spawn (_x, _y, _angle)
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

	--Player.debugtext = display.newText("NULL", 10, 10, native.systemfont, 10)
	--Player.debugtext.anchorX = 0; Player.debugtext.anchorY = 0
end

-- handler for key events (debug controls)
function Player:key (_event)
	if _event.phase == "down" then
		if     _event.keyName == "w"      then Player.controls.move.keys.up = true
		elseif _event.keyName == "d"      then Player.controls.move.keys.right = true
		elseif _event.keyName == "s"      then Player.controls.move.keys.down = true
		elseif _event.keyName == "a"      then Player.controls.move.keys.left = true
		elseif _event.keyName == "left"   then Player.controls.aim.keys.left = true
		elseif _event.keyName == "right"  then Player.controls.aim.keys.right = true
		elseif _event.keyName == "space"  then Player.controls.aim.keys.fire = true
		elseif _event.keyName == "escape" then Player.dead = true end
	elseif _event.phase == "up" then
		if     _event.keyName == "w"      then Player.controls.move.keys.up = false
		elseif _event.keyName == "d"      then Player.controls.move.keys.right = false
		elseif _event.keyName == "s"      then Player.controls.move.keys.down = false
		elseif _event.keyName == "a"      then Player.controls.move.keys.left = false
		elseif _event.keyName == "left"   then Player.controls.aim.keys.left = false
		elseif _event.keyName == "right"  then Player.controls.aim.keys.right = false
		elseif _event.keyName == "space"  then Player.controls.aim.keys.fire = false end
	end

	-- override default OS behaviour
	return true
end

-- handler for touch events (for the on-screen controls)
function Player:touch (_event)
	--Player.debugtext.text = "ID: " .. tostring(_event.id) .. "\nPos: " .. tostring(_event.x) .. ", " .. tostring(_event.y) .. "\nPhase: " .. _event.phase

	pos = { x = _event.x, y = _event.y }

	if _event.phase == "ended" or _event.phase == "cancelled" then
		if Collision.pointAndQuad(pos, Player.controls.move.bounds.all) then
			Player.controls.move.keys.up = false
			Player.controls.move.keys.right = false
			Player.controls.move.keys.down = false
			Player.controls.move.keys.left = false
		end
		if Collision.pointAndQuad(pos, Player.controls.aim.bounds.all) then
			Player.controls.aim.keys.left = false
			Player.controls.aim.keys.leftfire = false
			Player.controls.aim.keys.fire = false
			Player.controls.aim.keys.rightfire = false
			Player.controls.aim.keys.right = false
		end

		return true
	end

	if Collision.pointAndQuad(pos, Player.controls.move.bounds.up) then Player.controls.move.keys.up = true
	else Player.controls.move.keys.up = false end
	if Collision.pointAndQuad(pos, Player.controls.move.bounds.right) then Player.controls.move.keys.right = true
	else Player.controls.move.keys.right = false end
	if Collision.pointAndQuad(pos, Player.controls.move.bounds.down) then Player.controls.move.keys.down = true
	else Player.controls.move.keys.down = false end
	if Collision.pointAndQuad(pos, Player.controls.move.bounds.left) then Player.controls.move.keys.left = true
	else Player.controls.move.keys.left = false end

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

	return true
end

-- handler for updating the player each frame
function Player:enterFrame (_event)
	if Player.controls.aim.keys.left or Player.controls.aim.keys.leftfire then Player.angle = Player.angle + Constant.PLAYER_TURN_SPEED
	elseif Player.controls.aim.keys.right or Player.controls.aim.keys.rightfire then Player.angle = Player.angle - Constant.PLAYER_TURN_SPEED end
	Player.angle = Utility.capAngle(Player.angle)

	local dispx = 0
	local dispy = 0

	if Player.controls.move.keys.up then
		if Player.angle >= 0.0 and Player.angle < 180.0 then
			dispx = dispx + (math.cos(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy - (math.abs(math.sin(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED))
		else
			dispx = dispx + (math.cos(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy + (math.abs(math.sin(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED))
		end
	elseif Player.controls.move.keys.down then
		if Player.angle >= 0.0 and Player.angle < 180.0 then
			dispx = dispx - (math.cos(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy +  (math.abs(math.sin(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED))
		else
			dispx = dispx - (math.cos(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy - (math.abs(math.sin(math.rad(Player.angle)) * Constant.PLAYER_MOVE_SPEED))
		end
	end

	local tempangle
	if Player.controls.move.keys.left then tempangle = Utility.capAngle(Player.angle + 90.0)
	elseif Player.controls.move.keys.right then tempangle = Utility.capAngle(Player.angle - 90.0) end

	if Player.controls.move.keys.left or Player.controls.move.keys.right then
		if tempangle >= 0.0 and tempangle < 180.0 then
			dispx = dispx + (math.cos(math.rad(tempangle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy - (math.abs(math.sin(math.rad(tempangle)) * Constant.PLAYER_MOVE_SPEED))
		else
			dispx = dispx + (math.cos(math.rad(tempangle)) * Constant.PLAYER_MOVE_SPEED)
			dispy = dispy + (math.abs(math.sin(math.rad(tempangle)) * Constant.PLAYER_MOVE_SPEED))
		end
	end

	if Player.controls.aim.keys.leftfire or Player.controls.aim.keys.rightfire or Player.controls.aim.keys.fire then
		-- calculate if it hit, deduct health and add points
		if not Player.weapon.locked then
			for i = 1, Npclist.count do
				if Npclist.npcs[i].health > 0 then
					-- if it was a hit then the enemy is damaged (or killed) and the player gains points
					if Player.angle >= (Npclist.npcs[i].angle - Constant.AIM_EASING) and Player.angle <= (Npclist.npcs[i].angle + Constant.AIM_EASING) then
						Npclist.npcs[i].health = Npclist.npcs[i].health - 1
						audio.play(Familiar.sound)
						if Npclist.npcs[i].health > 0 then Player.score = Player.score + Constant.SCORE_HIT
						else
							Npclist.npcs[i].sprite.isVisible = false
							Player.score = Player.score + Constant.SCORE_KILL
						end
					end
				end
			end
		end

		Player.weapon:fire()
	end

	local collided = false
	local pos = { x = Player.x + dispx, y = Player.y + dispy }

	for iy = 1, Map.height do
		for ix = 1, Map.width do
			if Map[iy][ix] == Constant.TILE_SOLID then
				local tile = { x = (ix - 1) * Constant.TILE_WIDTH, y = (iy - 1) * Constant.TILE_WIDTH, w = Constant.TILE_WIDTH, h = Constant.TILE_WIDTH }
				if Collision.pointAndQuad(pos, tile) then
					dispx = 0; dispy = 0
					collided = true
					break
				end
			end
		end
		if collided then break end
	end

	Player.x = Player.x + dispx
	Player.y = Player.y + dispy

	local rayangle = Player.angle + (Constant.PLAYER_FOV / 2)

	for i = 1, Constant.PROJPLANE_WIDTH do
		rayangle = Utility.capAngle(rayangle)

		-- used to see if a ray is too long
		local horzmax = false
		local vertmax = false

		-- determine the vertical and horizontal direction of the current ray
		local horzdir;
		if rayangle >= 90.0 and rayangle < 270.0 then horzdir = Utility.dir.LEFT
		else horzdir = Utility.dir.RIGHT end
		local vertdir;
		if rayangle >= 0.0 and rayangle < 180.0 then vertdir = Utility.dir.UP
		else vertdir = Utility.dir.DOWN end

		-- calculate the first horizontal tile-intersection point
		local horzpos = { x = 0, y = 0 };
		if vertdir == Utility.dir.UP then horzpos.y = math.floor(Player.y / Constant.TILE_WIDTH) * (Constant.TILE_WIDTH) - 1
		else horzpos.y = math.floor(Player.y / Constant.TILE_WIDTH) * (Constant.TILE_WIDTH) + Constant.TILE_WIDTH end
		horzpos.x = Player.x + (Player.y - horzpos.y) / math.tan(math.rad(rayangle))

		-- if there is a wall at this point then stop checking horizontal intersections
		local horztile = { x = math.floor(horzpos.x / Constant.TILE_WIDTH) + 1, y = math.floor(horzpos.y / Constant.TILE_WIDTH) + 1 }
		if Utility.inBounds(horztile.x, 1, Map.width) and Utility.inBounds(horztile.y, 1, Map.height) then
			if Map[horztile.y][horztile.x] == Constant.TILE_EMPTY then
				-- however, if not we want to keep going until there is a wall (or max ray length is reached)
				local horzjump
				local vertjump
				if vertdir == Utility.dir.UP then
					vertjump = -Constant.TILE_WIDTH
					horzjump = Constant.TILE_WIDTH / math.tan(math.rad(rayangle))
				else
					vertjump = Constant.TILE_WIDTH
					horzjump = -(Constant.TILE_WIDTH / math.tan(math.rad(rayangle)))
				end
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

		-- calculate the first vertical tile-intersection point
		local vertpos = { x, y }
		if horzdir == Utility.dir.LEFT then vertpos.x = math.floor(Player.x / Constant.TILE_WIDTH) * (Constant.TILE_WIDTH) - 1
		else vertpos.x = math.floor(Player.x / Constant.TILE_WIDTH) * (Constant.TILE_WIDTH) + Constant.TILE_WIDTH end
		vertpos.y = Player.y + (Player.x - vertpos.x) * math.tan(math.rad(rayangle))

		-- if there is a wall at this point then stop checking vertical intersections
		local verttile = { x = math.floor(vertpos.x / Constant.TILE_WIDTH) + 1, y = math.floor(vertpos.y / Constant.TILE_WIDTH) + 1 }
		if Utility.inBounds(verttile.x, 1, Map.width) and Utility.inBounds(verttile.y, 1, Map.height) then
			if Map[verttile.y][verttile.x] == Constant.TILE_EMPTY then
				-- however, if not we want to keep going until there is a wall (or max ray length is reached)
				local vertjump
				local horzjump
				if horzdir == Utility.dir.LEFT then
					horzjump = -Constant.TILE_WIDTH
					vertjump = Constant.TILE_WIDTH * math.tan(math.rad(rayangle))
				else
					horzjump = Constant.TILE_WIDTH
					vertjump = -(Constant.TILE_WIDTH * math.tan(math.rad(rayangle)))
				end
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

		-- calculate the length of the horizontal and vertical tile-intersection rays
		local horzlen = Utility.length(Player.x, Player.y, horzpos.x, horzpos.y)
		local vertlen = Utility.length(Player.x, Player.y, vertpos.x, vertpos.y)

		-- determine which is shorter then remove the fishbowl and store in the ray table
		if math.abs(horzlen) <= math.abs(vertlen) then
			if horzmax then Raycast.rays[i] = Constant.RAY_OUT_OF_BOUNDS
			else
				Raycast.rays[i] = math.abs(horzlen * math.cos(math.rad(-(Constant.PLAYER_FOV / 2) + (Constant.RAY_ANGLE * (i - 1)))))
				Raycast.offsets[i] = math.floor(horzpos.x % Constant.TILE_WIDTH)
				Raycast.dirs[i] = "horz"
			end
		else
			if vertmax then Raycast.rays[i] = Constant.RAY_OUT_OF_BOUNDS
			else
				Raycast.rays[i] = math.abs(vertlen * math.cos(math.rad(-(Constant.PLAYER_FOV / 2) + (Constant.RAY_ANGLE * (i - 1)))))
				Raycast.offsets[i] = math.floor(vertpos.y % Constant.TILE_WIDTH)
				Raycast.dirs[i] = "vert"
			end
		end

		-- move on to the next ray for casting
		rayangle = rayangle - Constant.RAY_ANGLE
	end
end
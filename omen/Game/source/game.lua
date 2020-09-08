--[[
Game script contains all the code for the game, wrapped-up inside
the game scene. This scene is the third in the main application
order; the second is the menu, the first is the splash screen, and
that is how the game scene is reached.

Everything to do with the game whilst it is being played is
handled inside of this scene.
--]]

-- includes functionality from other scripts
require("source.utility")
require("source.constant")
require("source.letterbox")
require("source.npclist")
require("source.player")
require("source.map")
require("source.npc")

-- creates the game scene via a scene composer
local composer = require("composer")
composer.recycleOnSceneChange = true -- clears the scene from memory after use
local scene = composer.newScene()

--[[
This functions stores the code to be run on start-up when the game scene
is loaded into the composer. It sets up everything for the game to be
played, such as: the game's graphics, loads in the map to use, sets up
the player character, adds relevant event listeners, starts music, etc.

This code is automatically called each time the game scene is loaded and
is cleaned up via the appropriate scene destroy function.
--]]

function scene:create (_event)
	-- initialises the letter boxing borders for use in the game scene
	Letterbox.init()

	-- loads in the background elements of the game world (so they are rendered behind everything)
	local skybox = display.newImageRect("assets/images/skybox.png", 640, 120)
	skybox.x = Constant.CONTENT_CENTER_X; skybox.y = Constant.CONTENT_HEIGHT / 4
	skybox:setFillColor(0.4, 0.4, 0.4)
	local floor = display.newImageRect("assets/images/floor.png", 640, 120)
	floor.x = Constant.CONTENT_CENTER_X; floor.y = Constant.CONTENT_HEIGHT - (120 / 2)

	-- loads and parses the specified map file into usable data for the game
	Map.load("arena.map")

	-- spawns the player in at the correct constant start position and angle
	Player.spawn(Constant.PLAYER_START_X, Constant.PLAYER_START_Y, Constant.PLAYER_START_ANGLE)

	-- preloads the game over image for later, but does not show it yet
	local gameover = display.newImageRect("assets/images/gameover.png", 320, 240)
	gameover.x = Constant.CONTENT_CENTER_X; gameover.y = Constant.CONTENT_CENTER_Y
	gameover.isVisible = false

	-- the game over buttons are also preloaded, but once again not shown yet
	local restartbutton = display.newImageRect("assets/images/button_restart.png", 32, 32)
	restartbutton.x = Constant.CONTENT_CENTER_X - 32; restartbutton.y = Constant.CONTENT_CENTER_Y + 48
	restartbutton.isVisible = false
	local menubutton = display.newImageRect("assets/images/button_menu.png", 32, 32)
	menubutton.x = Constant.CONTENT_CENTER_X + 32; menubutton.y = Constant.CONTENT_CENTER_Y + 48
	menubutton.isVisible = false

	-- the same preloading goes for the game over score text
	local scoretext = display.newText({
		text = "NULL",
		x = Constant.CONTENT_CENTER_X, y = Constant.CONTENT_CENTER_Y,
		font = native.systemFont,
		fontSize = 12
	})
	scoretext.isVisible = false
	scoretext:setFillColor(215 / 255, 174 / 255, 39 / 255)

	-- the graphical elements of the game are added to the current scene display group (corona recommends)
	self.view:insert(skybox)
	self.view:insert(floor)
	for i = 1, Constant.PROJPLANE_WIDTH do self.view:insert(Map.slices[i]) end
	self.view:insert(Player.weapon.sprite)
	self.view:insert(Player.controls.move)
	self.view:insert(Player.controls.aim)
	self.view:insert(gameover)
	self.view:insert(restartbutton)
	self.view:insert(menubutton)
	self.view:insert(scoretext)
	
	-- not all boundaries may have been initialised, based on resolution, so these if checks are important
	if Letterbox.bleed.left ~= nil then self.view:insert(Letterbox.bleed.left) end
	if Letterbox.bleed.right ~= nil then self.view:insert(Letterbox.bleed.right) end
	if Letterbox.bleed.top ~= nil then self.view:insert(Letterbox.bleed.top) end
	if Letterbox.bleed.bottom ~= nil then self.view:insert(Letterbox.bleed.bottom) end

	-- the spawn counters are set, so enemies spawn correctly, it balances difficulty
	local spawner = {}
	spawner[1] = Constant.SPAWN_TIME_START_1
	spawner[2] = Constant.SPAWN_TIME_START_2
	spawner[3] = Constant.SPAWN_TIME_START_3

	--[[
	The enter frame event handler function for the game scene is used to update
	everything currently within the game scene. It handles the spawning/creation
	of new NPCs into the scene, the updating of where to render the currently
	visible NPC graphics, and what to do when the player dies (clean-up, etc).

	This function is called upon every single frame to ensure that the game is
	running correctly and keeps everything managed.
	--]]

	function scene:enterFrame (_event)
		-- handle the player's death and sets up the game over screen
		if Player.dead then
			-- layers the death sound to make it louder
			audio.play(Player.sound); audio.play(Player.sound)
			-- stops the audio on channel 1 (the music channel)
			audio.stop(1)

			-- removes all the game's event listeners so the game no longer runs whilst dead
			Runtime:removeEventListener("key", Player)
			Runtime:removeEventListener("touch", Player)
			Runtime:removeEventListener("enterFrame", Player)
			Runtime:removeEventListener("enterFrame", self)
			Runtime:removeEventListener("enterFrame", Map)

			-- begins the highscore saving/loading system
			local highscore = 0
			local path = system.pathForFile("highscore.dat")
			
			-- loads the current highscore from a file to be compared
			local ifile, ierror = io.open(path, "r")
			if not ifile then error("ERROR: " .. ierror) end
			highscore = tonumber(ifile:read("*l"))
			ifile:close()

			-- if the new score is better than the old highscore it is saved
			if Player.score > highscore then
				highscore = Player.score
				local ofile, oerror = io.open(path, "w")
				if not ofile then error("ERROR: " .. oerror) end
				ofile:write(tostring(Player.score))
				ofile:close()
			end

			-- the score and highscore are then printed to the game over screen
			scoretext.text = "Your score: " .. tostring(Player.score) .. "\nHighscore: " .. tostring(highscore)

			-- shows all the game over GUI elements
			gameover.isVisible = true
			restartbutton.isVisible = true
			menubutton.isVisible = true
			scoretext.isVisible = true
			-- pushes them to the front so they are visible above all
			gameover:toFront()
			restartbutton:toFront()
			menubutton:toFront()
			scoretext:toFront()

			-- leaves the update function early (no need to run the rest)
			return
		end

		-- goes through the three spawners and spawns new enemies into the game
		for i = 1, Constant.SPAWNER_TOTAL do
			-- checks the current spawner's counter is finished
			if spawner[i] <= 0 then
				-- if it is then a new enemy is added into the NPC list for use
				Npclist.count = Npclist.count + 1
				Npclist.npcs[Npclist.count] = Familiar(Constant.SPAWNER_POS_X[i], Constant.SPAWNER_POS_Y[i])
				self.view:insert(Npclist.npcs[Npclist.count].sprite)
				Runtime:addEventListener("enterFrame", Npclist.npcs[Npclist.count])
				-- the counter is reset for a new spawn
				spawner[i] = math.random(Constant.SPAWN_TIME_MIN, Constant.SPAWN_TIME_MAX)
			else
				-- if the counter is not done it ticks down
				spawner[i] = spawner[i] - 1
			end
		end

		-- updates and renders every single NPC that is currently in play
		for i = 1, Npclist.count do
			-- it does not bother updating dead NPCs
			if Npclist.npcs[i].health > 0 then
				-- uses the player's and NPC's angle to calculate where it should be on screen
				local tempy = Player.angle + (Constant.PLAYER_FOV / 2) - Npclist.npcs[i].angle
				-- wraps around if necessessary, to avoid calculation errors
				if Npclist.npcs[i].angle > 270.0 and Player.angle < 90.0 then tempy = Player.angle + (Constant.PLAYER_FOV / 2) - Npclist.npcs[i].angle + 360.0 end
				if Player.angle > 270.0 and Npclist.npcs[i].angle < 90.0 then tempy = Player.angle + (Constant.PLAYER_FOV / 2) - Npclist.npcs[i].angle - 360.0 end
				-- uses the angles to determine the X position to draw at and the distance to determine the scale
				Npclist.npcs[i].sprite.x = tempy * Constant.PROJPLANE_WIDTH / Constant.PLAYER_FOV
				Npclist.npcs[i].sprite.width = math.ceil(Familiar.spritewidth / Npclist.npcs[i].distance * Constant.PROJPLANE_DISTANCE)
				Npclist.npcs[i].sprite.height = math.ceil(Familiar.spriteheight / Npclist.npcs[i].distance * Constant.PROJPLANE_DISTANCE)
				-- sets the brightness according to how far away the NPC is
				local shadow = 1.0 - (Npclist.npcs[i].distance / Constant.NPC_DISTANCE_SHADOW)
				Npclist.npcs[i].sprite:setFillColor(shadow, shadow, shadow)

				-- creates a bounding quad and checks to see if the player is colliding with the NPC
				local bounds = {
					x = Npclist.npcs[i].x - (Constant.NPC_FAMILIAR_BOUNDS / 2),
					y = Npclist.npcs[i].y - (Constant.NPC_FAMILIAR_BOUNDS / 2),
					w = Constant.NPC_FAMILIAR_BOUNDS,
					h = Constant.NPC_FAMILIAR_BOUNDS
				}
				-- if so then the player dies
				if Collision.pointAndQuad({ x = Player.x, y = Player.y }, bounds) then Player.dead = true end
			end
		end

		-- moves the GUI and letter box bounds to the front so NPCs don't draw in front
		Player.weapon.sprite:toFront()
		Player.controls.aim:toFront()
		Player.controls.move:toFront()
		-- not all boundaries may have been initialised, based on resolution, so these if checks are important
		if Letterbox.bleed.left ~= nil then Letterbox.bleed.left:toFront() end
		if Letterbox.bleed.right ~= nil then Letterbox.bleed.right:toFront() end
		if Letterbox.bleed.top ~= nil then Letterbox.bleed.top:toFront() end
		if Letterbox.bleed.bottom ~= nil then Letterbox.bleed.bottom:toFront() end
	end

	--[[
	These functions are the event handlers for the game over buttons. They transition
	to either the restart or menu scenes, via a composer, based on which button is
	pressed. The reason restarting goes to a new scene instead of the game is due to
	how composers work in Corona, it is not possible to go to the current scene, so a
	separate one is required.
	--]]

	function restartbutton:tap (_event) composer.gotoScene("source.restart", { effect = "fade", time = 0 }) end
	function menubutton:tap (_event) composer.gotoScene("source.menu", { effect = "fade", time = 1000 }) end

	-- adds all the neccessary event listeners for the game to work, the handlers are called each frame
	Runtime:addEventListener("key", Player)
	Runtime:addEventListener("touch", Player)
	Runtime:addEventListener("enterFrame", Player)
	Runtime:addEventListener("enterFrame", Map)
	Runtime:addEventListener("enterFrame", self)
	-- adds the event listeners for the game over buttons, the handlers are called on tap
	restartbutton:addEventListener("tap", restartbutton)
	menubutton:addEventListener("tap", menubutton)

	-- loads in one of the three music tracks randomly and begins playing
	local music = audio.loadStream("assets/music/" .. tostring(math.random(1, Constant.MUSIC_TOTAL)) .. ".mp3")
	audio.play(music, { channel = 1, loops = -1 })
end

--[[
Once the scene is no longer needed and the composer transitions to a new
scene, this code is run to clear up memory and avoid memory leaks. This
function ensures that everything created with the scene is destroyed with
it. The function will be called on a composer.goto() function call.
--]]

function scene:destroy (_event)
	-- clears all the NPCs from existance, stops them from updating
	for i = 1, Npclist.count do Runtime:removeEventListener("enterFrame", Npclist.npcs[i]) end
	Npclist.npcs = {}
	Npclist.count = 0
	-- disposes of audio (very important as Corona will not do this automatically)
	audio.dispose(Player.weapon.sound)
	audio.dispose(Player.sound)
	audio.dispose(music)
	-- removes the current display group, clearing all the graphics
	display.remove(self.view)
end

-- adds the neccessary event listeners for the current scene to function
scene:addEventListener("create", scene)
scene:addEventListener("destroy", scene)

-- returns the scene so it can be used by a composer
return scene
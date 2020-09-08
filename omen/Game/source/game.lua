--[[
Game script contains all the code for the game, wrapped-up inside
the game scene. This scene is the third in the main application
order; the second is the menu, the first is the splash screen, and
that is how the game scene is reached.

Everything to do with the game whilst it is being played is
handled inside of this scene.
--]]

require("source.utility")
require("source.constant")
require("source.letterbox")
require("source.npclist")
require("source.player")
require("source.map")
require("source.npc")

local composer = require("composer")
composer.recycleOnSceneChange = true
local scene = composer.newScene()

function scene:create (_event)
	Letterbox.init()

	local skybox = display.newImageRect("assets/images/skybox.png", 640, 120)
	skybox.x = Constant.CONTENT_CENTER_X; skybox.y = Constant.CONTENT_HEIGHT / 4
	skybox:setFillColor(0.4, 0.4, 0.4)
	self.view:insert(skybox)

	local floor = display.newImageRect("assets/images/floor.png", 640, 120)
	floor.x = Constant.CONTENT_CENTER_X; floor.y = Constant.CONTENT_HEIGHT - (120 / 2)
	self.view:insert(floor)

	Map.load("arena.map")
	for i = 1, Constant.PROJPLANE_WIDTH do self.view:insert(Map.slices[i]) end

	Player.spawn(Constant.PLAYER_START_X, Constant.PLAYER_START_Y, Constant.PLAYER_START_ANGLE)
	self.view:insert(Player.weapon.sprite)
	self.view:insert(Player.controls.move)
	self.view:insert(Player.controls.aim)

	local gameover = display.newImageRect("assets/images/gameover.png", 320, 240)
	gameover.x = Constant.CONTENT_CENTER_X; gameover.y = Constant.CONTENT_CENTER_Y
	gameover.isVisible = false
	self.view:insert(gameover)

	local restartbutton = display.newImageRect("assets/images/button_restart.png", 32, 32)
	restartbutton.x = Constant.CONTENT_CENTER_X - 32; restartbutton.y = Constant.CONTENT_CENTER_Y + 48
	restartbutton.isVisible = false
	self.view:insert(restartbutton)
	local menubutton = display.newImageRect("assets/images/button_menu.png", 32, 32)
	menubutton.x = Constant.CONTENT_CENTER_X + 32; menubutton.y = Constant.CONTENT_CENTER_Y + 48
	menubutton.isVisible = false
	self.view:insert(menubutton)

	local scoretext = display.newText({
		text = "NULL",
		x = Constant.CONTENT_CENTER_X, y = Constant.CONTENT_CENTER_Y,
		font = native.systemFont,
		fontSize = 12
	})
	scoretext.isVisible = false
	scoretext:setFillColor(215 / 255, 174 / 255, 39 / 255)
	self.view:insert(scoretext)

	if Letterbox.bleed.left ~= nil then self.view:insert(Letterbox.bleed.left) end
	if Letterbox.bleed.right ~= nil then self.view:insert(Letterbox.bleed.right) end
	if Letterbox.bleed.top ~= nil then self.view:insert(Letterbox.bleed.top) end
	if Letterbox.bleed.bottom ~= nil then self.view:insert(Letterbox.bleed.bottom) end

	local spawner = {}
	spawner[1] = Constant.SPAWN_TIME_START_1
	spawner[2] = Constant.SPAWN_TIME_START_2
	spawner[3] = Constant.SPAWN_TIME_START_3

	function scene:enterFrame (_event)
		-- handle the player's death
		if Player.dead then
			audio.play(Player.sound); audio.play(Player.sound) -- probably not the best idea
			audio.stop(1)

			Runtime:removeEventListener("key", Player)
			Runtime:removeEventListener("touch", Player)
			Runtime:removeEventListener("enterFrame", Player)
			Runtime:removeEventListener("enterFrame", self)
			Runtime:removeEventListener("enterFrame", Map)

			local highscore = 0
			local path = system.pathForFile("highscore.dat")
			
			local ifile, ierror = io.open(path, "r")
			if not ifile then error("ERROR: " .. ierror) end
			highscore = tonumber(ifile:read("*l"))
			ifile:close()

			if Player.score > highscore then
				highscore = Player.score
				local ofile, oerror = io.open(path, "w")
				if not ofile then error("ERROR: " .. oerror) end
				ofile:write(tostring(Player.score))
				ofile:close()
			end

			scoretext.text = "Your score: " .. tostring(Player.score) .. "\nHighscore: " .. tostring(highscore)

			gameover.isVisible = true
			gameover:toFront();
			restartbutton.isVisible = true
			restartbutton:toFront()
			menubutton.isVisible = true
			menubutton:toFront()
			scoretext.isVisible = true
			scoretext:toFront()

			return
		end

		-- spawn in new enemies
		for i = 1, Constant.SPAWNER_TOTAL do
			if spawner[i] <= 0 then
				Npclist.count = Npclist.count + 1
				Npclist.npcs[Npclist.count] = Familiar(Constant.SPAWNER_POS_X[i], Constant.SPAWNER_POS_Y[i])
				self.view:insert(Npclist.npcs[Npclist.count].sprite)
				Runtime:addEventListener("enterFrame", Npclist.npcs[Npclist.count])
				spawner[i] = math.random(Constant.SPAWN_TIME_MIN, Constant.SPAWN_TIME_MAX)
			else
				spawner[i] = spawner[i] - 1
			end
		end

		for i = 1, Npclist.count do
			if Npclist.npcs[i].health > 0 then
				local tempy = Player.angle + (Constant.PLAYER_FOV / 2) - Npclist.npcs[i].angle
				if Npclist.npcs[i].angle > 270.0 and Player.angle < 90.0 then tempy = Player.angle + (Constant.PLAYER_FOV / 2) - Npclist.npcs[i].angle + 360.0 end
				if Player.angle > 270.0 and Npclist.npcs[i].angle < 90.0 then tempy = Player.angle + (Constant.PLAYER_FOV / 2) - Npclist.npcs[i].angle - 360.0 end
				Npclist.npcs[i].sprite.x = tempy * Constant.PROJPLANE_WIDTH / Constant.PLAYER_FOV
				Npclist.npcs[i].sprite.width = math.ceil(Familiar.spritewidth / Npclist.npcs[i].distance * Constant.PROJPLANE_DISTANCE)
				Npclist.npcs[i].sprite.height = math.ceil(Familiar.spriteheight / Npclist.npcs[i].distance * Constant.PROJPLANE_DISTANCE)
				local shadow = 1.0 - (Npclist.npcs[i].distance / Constant.NPC_DISTANCE_SHADOW)
				Npclist.npcs[i].sprite:setFillColor(shadow, shadow, shadow)

				local bounds = {
					x = Npclist.npcs[i].x - (Constant.NPC_FAMILIAR_BOUNDS / 2),
					y = Npclist.npcs[i].y - (Constant.NPC_FAMILIAR_BOUNDS / 2),
					w = Constant.NPC_FAMILIAR_BOUNDS,
					h = Constant.NPC_FAMILIAR_BOUNDS
				}
				if Collision.pointAndQuad({ x = Player.x, y = Player.y }, bounds) then Player.dead = true end
			end
		end

		-- move all neccessary images to the front
		Player.weapon.sprite:toFront()
		Player.controls.aim:toFront()
		Player.controls.move:toFront()
		if Letterbox.bleed.left ~= nil then Letterbox.bleed.left:toFront() end
		if Letterbox.bleed.right ~= nil then Letterbox.bleed.right:toFront() end
		if Letterbox.bleed.top ~= nil then Letterbox.bleed.top:toFront() end
		if Letterbox.bleed.bottom ~= nil then Letterbox.bleed.bottom:toFront() end
	end

	function restartbutton:tap (_event) composer.gotoScene("source.restart", { effect = "fade", time = 0 }) end
	function menubutton:tap (_event) composer.gotoScene("source.menu", { effect = "fade", time = 1000 }) end

	Runtime:addEventListener("key", Player)
	Runtime:addEventListener("touch", Player)
	Runtime:addEventListener("enterFrame", Player)
	Runtime:addEventListener("enterFrame", Map)
	Runtime:addEventListener("enterFrame", self)

	restartbutton:addEventListener("tap", restartbutton)
	menubutton:addEventListener("tap", menubutton)

	local music = audio.loadStream("assets/music/" .. tostring(math.random(1, Constant.MUSIC_TOTAL)) .. ".mp3")
	audio.play(music, { channel = 1, loops = -1 })
end

function scene:destroy (_event)
	for i = 1, Npclist.count do Runtime:removeEventListener("enterFrame", Npclist.npcs[i]) end
	Npclist.npcs = {}
	Npclist.count = 0
	display.remove(self.view)
	audio.dispose(Player.weapon.sound)
	audio.dispose(Player.sound)
	audio.dispose(music)
end

scene:addEventListener("create", scene)
scene:addEventListener("destroy", scene)

return scene
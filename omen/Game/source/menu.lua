--[[
Menu script contains all the code for the menu, wrapped-up inside
the menu scene. This scene is the second in the application, when
the game loads it boots directly into the menu after displaying
the splash screen for a short period first.

Everything to do with the main menu and its various sub-menus
(help and settings) is handled inside of this scene.
--]]

require("source.constant")
require("source.letterbox")

local composer = require("composer")
composer.recycleOnSceneChange = true
local scene = composer.newScene()

function scene:create (_event)
	Letterbox.init()
	
	local background = display.newImageRect("assets/images/titlescreen.png", 320, 240)
	background.x = Constant.CONTENT_CENTER_X; background.y = Constant.CONTENT_CENTER_Y
	
	local playbutton = display.newImageRect("assets/images/button_play.png", 64, 56)
	playbutton.x = 162; playbutton.y = 210
	local helpbutton = display.newImageRect("assets/images/button_help.png", 48, 32)
	helpbutton.x = 64; helpbutton.y = 210
	local settingsbutton = display.newImageRect("assets/images/button_settings.png", 72, 32)
	settingsbutton.x = 256; settingsbutton.y = 210
	local creditsbutton = display.newImageRect("assets/images/button_credits.png", 72, 32)
	creditsbutton.x = 284; creditsbutton.y = 16
	local backbutton = display.newImageRect("assets/images/button_back.png", 48, 32)
	backbutton.x = 34; backbutton.y = 26
	backbutton.isVisible = false

	local help = display.newImageRect("assets/images/help.png", 304, 144)
	help.x = 160; help.y = 160
	help.isVisible = false

	local settings = {}
	settings.spritewidth = 48; settings.spriteheight = 48
	settings.spritesheet = graphics.newImageSheet(
		"assets/images/settings.png",
		{ width = settings.spritewidth, height = settings.spriteheight, numFrames = 4, sheetContentWidth = 192, sheetContentHeight = 48 }
	)
	settings.animations = {
		{ name = "sound_on", start = 1, count = 1, time = 0, loopCount = 0, loopDirection = "forward" },
		{ name = "sound_off", start = 2, count = 1, time = 0, loopCount = 0, loopDirection = "forward" },
		{ name = "music_on", start = 3, count = 1, time = 0, loopCount = 0, loopDirection = "forward" },
		{ name = "music_off", start = 4, count = 1, time = 0, loopCount = 0, loopDirection = "forward" }
	}

	local soundbutton = display.newSprite(settings.spritesheet, settings.animations)
	soundbutton.x = 105; soundbutton.y = 149
	soundbutton:setSequence("sound_on")
	soundbutton.isVisible = false
	local musicbutton = display.newSprite(settings.spritesheet, settings.animations)
	musicbutton.x = 213; musicbutton.y = 149
	musicbutton:setSequence("music_on")
	musicbutton.isVisible = false

	local credits = display.newImageRect("assets/images/credits.png", 304, 144)
	credits.x = 160; credits.y = 160
	credits.isVisible = false

	self.view:insert(background)
	self.view:insert(playbutton)
	self.view:insert(helpbutton)
	self.view:insert(settingsbutton)
	self.view:insert(creditsbutton)
	self.view:insert(backbutton)
	self.view:insert(help)
	self.view:insert(soundbutton)
	self.view:insert(musicbutton)
	self.view:insert(credits)

	if Letterbox.bleed.left ~= nil then self.view:insert(Letterbox.bleed.left) end
	if Letterbox.bleed.right ~= nil then self.view:insert(Letterbox.bleed.right) end
	if Letterbox.bleed.top ~= nil then self.view:insert(Letterbox.bleed.top) end
	if Letterbox.bleed.bottom ~= nil then self.view:insert(Letterbox.bleed.bottom) end

	function playbutton:tap (_event)
		composer.gotoScene("source.game", { effect = "zoomInOutFade", time = 1000 })
	end

	function settingsbutton:tap (_event)
		playbutton.isVisible = false
		helpbutton.isVisible = false
		settingsbutton.isVisible = false
		creditsbutton.isVisible = false
		backbutton.isVisible = true
		soundbutton.isVisible = true
		musicbutton.isVisible = true
	end

	function helpbutton:tap (_event)
		playbutton.isVisible = false
		helpbutton.isVisible = false
		settingsbutton.isVisible = false
		creditsbutton.isVisible = false
		backbutton.isVisible = true
		help.isVisible = true
	end

	function creditsbutton:tap (_event)
		playbutton.isVisible = false
		helpbutton.isVisible = false
		settingsbutton.isVisible = false
		creditsbutton.isVisible = false
		backbutton.isVisible = true
		credits.isVisible = true
	end

	function backbutton:tap (_event)
		playbutton.isVisible = true
		helpbutton.isVisible = true
		settingsbutton.isVisible = true
		creditsbutton.isVisible = true
		backbutton.isVisible = false
		help.isVisible = false
		soundbutton.isVisible = false
		musicbutton.isVisible = false
		credits.isVisible = false
	end

	function soundbutton:tap (_event)
		if self.sequence == "sound_on" then
			self:setSequence("sound_off")
			for i = 2, Constant.MAX_AUDIO_CHANNELS do audio.setVolume(0.0, { channel = i }) end
		else
			self:setSequence("sound_on")
			for i = 2, Constant.MAX_AUDIO_CHANNELS do audio.setVolume(1.0, { channel = i }) end
		end
	end

	function musicbutton:tap (_event)
		if self.sequence == "music_on" then
			self:setSequence("music_off")
			audio.setVolume(0.0, { channel = 1 })
		else
			self:setSequence("music_on")
			audio.setVolume(1.0, { channel = 1 })
		end
	end

	playbutton:addEventListener("tap", playbutton)
	helpbutton:addEventListener("tap", helpbutton)
	settingsbutton:addEventListener("tap", settingsbutton)
	creditsbutton:addEventListener("tap", creditsbutton)
	backbutton:addEventListener("tap", backbutton)
	soundbutton:addEventListener("tap", soundbutton)
	musicbutton:addEventListener("tap", musicbutton)
end

function scene:destroy (_event)
	display.remove(self.view)
end

scene:addEventListener("create", scene)
scene:addEventListener("destroy", scene)

return scene
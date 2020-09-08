--[[
Menu script contains all the code for the menu, wrapped-up inside
the menu scene. This scene is the second in the application, when
the game loads it boots directly into the menu after displaying
the splash screen for a short period first.

Everything to do with the main menu and its various sub-menus
(help and settings) is handled inside of this scene.
--]]

-- includes functionality from other scripts
require("source.constant")
require("source.letterbox")

-- creates the menu scene via a scene composer
local composer = require("composer")
composer.recycleOnSceneChange = true -- clears the scene from memory after use
local scene = composer.newScene()

--[[
This functions stores the code to be run on start-up when the menu scene
is loaded into the composer. It sets up all the graphics and event listeners+
handlers to work correctly.

This code is automatically called each time the menu scene is loaded and
is cleaned up via the appropriate scene destroy function.
--]]

function scene:create (_event)
	-- initialises the letter boxing borders for use in the game scene
	Letterbox.init()
	
	-- loads the background+title image for the main menu, used in all sub-menus
	local background = display.newImageRect("assets/images/titlescreen.png", 320, 240)
	background.x = Constant.CONTENT_CENTER_X; background.y = Constant.CONTENT_CENTER_Y
	
	-- preloads all the menu buttons, even those not in use like the back button
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

	-- preloads the help box on the help sub-menu, will not be visible straight away
	local help = display.newImageRect("assets/images/help.png", 304, 144)
	help.x = 160; help.y = 160
	help.isVisible = false

	-- creates the sprite sheet and sprites for the sound+music buttons on the settings menu
	local settings = {}
	settings.spritewidth = 48; settings.spriteheight = 48
	settings.spritesheet = graphics.newImageSheet(
		"assets/images/settings.png",
		{ width = settings.spritewidth, height = settings.spriteheight, numFrames = 4, sheetContentWidth = 192, sheetContentHeight = 48 }
	)
	-- each button has two possible states, on or off
	settings.animations = {
		{ name = "sound_on", start = 1, count = 1, time = 0, loopCount = 0, loopDirection = "forward" },
		{ name = "sound_off", start = 2, count = 1, time = 0, loopCount = 0, loopDirection = "forward" },
		{ name = "music_on", start = 3, count = 1, time = 0, loopCount = 0, loopDirection = "forward" },
		{ name = "music_off", start = 4, count = 1, time = 0, loopCount = 0, loopDirection = "forward" }
	}

	-- preloads the to settings buttons for the settings sub-menu, will not be visible straight away
	local soundbutton = display.newSprite(settings.spritesheet, settings.animations)
	soundbutton.x = 105; soundbutton.y = 149
	soundbutton:setSequence("sound_on")
	soundbutton.isVisible = false
	local musicbutton = display.newSprite(settings.spritesheet, settings.animations)
	musicbutton.x = 213; musicbutton.y = 149
	musicbutton:setSequence("music_on")
	musicbutton.isVisible = false

	-- preloads the credits box on the credits sub-menu, will not be visible straight away
	local credits = display.newImageRect("assets/images/credits.png", 304, 144)
	credits.x = 160; credits.y = 160
	credits.isVisible = false

	-- the graphical elements of the menu are added to the current scene display group (corona recommends)
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

	-- not all boundaries may have been initialised, based on resolution, so these if checks are important
	if Letterbox.bleed.left ~= nil then self.view:insert(Letterbox.bleed.left) end
	if Letterbox.bleed.right ~= nil then self.view:insert(Letterbox.bleed.right) end
	if Letterbox.bleed.top ~= nil then self.view:insert(Letterbox.bleed.top) end
	if Letterbox.bleed.bottom ~= nil then self.view:insert(Letterbox.bleed.bottom) end

	--[[
	This event handler for the play button transitions from the current menu scene
	into the game scene. Calling the scene destroy function to clean-up.
	--]]

	function playbutton:tap (_event)
		composer.gotoScene("source.game", { effect = "zoomInOutFade", time = 1000 })
	end

	--[[
	This event handler for the settings button hides all the current menu GUI
	(freezing their event listeners) and reveals the settings sub-menu GUI.
	--]]

	function settingsbutton:tap (_event)
		playbutton.isVisible = false
		helpbutton.isVisible = false
		settingsbutton.isVisible = false
		creditsbutton.isVisible = false
		backbutton.isVisible = true
		soundbutton.isVisible = true
		musicbutton.isVisible = true
	end

	--[[
	This event handler for the settings button hides all the current menu GUI
	(freezing their event listeners) and reveals the help sub-menu GUI.
	--]]

	function helpbutton:tap (_event)
		playbutton.isVisible = false
		helpbutton.isVisible = false
		settingsbutton.isVisible = false
		creditsbutton.isVisible = false
		backbutton.isVisible = true
		help.isVisible = true
	end

	--[[
	This event handler for the settings button hides all the current menu GUI
	(freezing their event listeners) and reveals the credits sub-menu GUI.
	--]]

	function creditsbutton:tap (_event)
		playbutton.isVisible = false
		helpbutton.isVisible = false
		settingsbutton.isVisible = false
		creditsbutton.isVisible = false
		backbutton.isVisible = true
		credits.isVisible = true
	end

	--[[
	This event handdler for the back button hides all the sub-menu GUI for the
	settings, help, and credits sub-menus and reveals the menu GUI once again.
	This creates a generalised backbutton that can be used to return from any of
	the sub-menus.
	--]]

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

	--[[
	This event handler for the sound button is used to mute/un-mute all the sound
	channels (every channel between 2-32).
	--]]

	function soundbutton:tap (_event)
		if self.sequence == "sound_on" then
			-- if the sound is currently on then it is muted and the muted sprite is displayed
			self:setSequence("sound_off")
			for i = 2, Constant.MAX_AUDIO_CHANNELS do audio.setVolume(0.0, { channel = i }) end
		else
			-- if the sound is currently off then it is un-muted and the un-muted sprite is displayed
			self:setSequence("sound_on")
			for i = 2, Constant.MAX_AUDIO_CHANNELS do audio.setVolume(1.0, { channel = i }) end
		end
	end

	--[[
	This event handler for the music button is used to mute/un-mute the music channel
	(the first channel).
	--]]

	function musicbutton:tap (_event)
		if self.sequence == "music_on" then
			-- if the music is currently on then it is muted and the muted sprite is displayed
			self:setSequence("music_off")
			audio.setVolume(0.0, { channel = 1 })
		else
			-- if the music is currently off then it is un-muted and the un-muted sprite is displayed
			self:setSequence("music_on")
			audio.setVolume(1.0, { channel = 1 })
		end
	end

	-- adds all the neccessary event listeners for the menu to work, the handlers are called on tap
	playbutton:addEventListener("tap", playbutton)
	helpbutton:addEventListener("tap", helpbutton)
	settingsbutton:addEventListener("tap", settingsbutton)
	creditsbutton:addEventListener("tap", creditsbutton)
	backbutton:addEventListener("tap", backbutton)
	soundbutton:addEventListener("tap", soundbutton)
	musicbutton:addEventListener("tap", musicbutton)
end

--[[
Once the scene is no longer needed and the composer transitions to a new
scene, this code is run to clear up memory and avoid memory leaks. This
function ensures that everything created with the scene is destroyed with
it. The function will be called on a composer.goto() function call.
--]]

function scene:destroy (_event)
	-- removes the current display group, clearing all the graphics
	display.remove(self.view)
end

-- adds the neccessary event listeners for the current scene to function
scene:addEventListener("create", scene)
scene:addEventListener("destroy", scene)

-- returns the scene so it can be used by a composer
return scene
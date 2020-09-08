--[[
Splash script contains all the code for the splash screen, wrapped-up
inside the splash scene. This scene is the first in the application,
when the game loads it boots directly into the splash screen for a
short period. After a while it will load the menu up.
--]]

-- includes functionality from other scripts
require("source.constant")
require("source.letterbox")

-- creates the menu scene via a scene composer
local composer = require("composer")
composer.recycleOnSceneChange = true -- clears the scene from memory after use
local scene = composer.newScene()

--[[
This function is called when the splash scene is created, it is used to
load in the splash screen image and the callback that will be called once
the splash timer has finished counting down.
--]]

function scene:create (_event)
	-- loads in the backgroud graphic for the splash screen
	local background = display.newImageRect("assets/images/titlescreen.png", 320, 240)
	background.x = Constant.CONTENT_CENTER_X; background.y = Constant.CONTENT_CENTER_Y

	-- the splash screen graphic is added to the current scene display group (corona recommends)
	self.view:insert(background)

	-- not all boundaries may have been initialised, based on resolution, so these if checks are important
	if Letterbox.bleed.left ~= nil then self.view:insert(Letterbox.bleed.left) end
	if Letterbox.bleed.right ~= nil then self.view:insert(Letterbox.bleed.right) end
	if Letterbox.bleed.top ~= nil then self.view:insert(Letterbox.bleed.top) end
	if Letterbox.bleed.bottom ~= nil then self.view:insert(Letterbox.bleed.bottom) end

	--[[
	The transition function is used to go from the splash screen scene into
	the menu scene, beginning the control of the user and start of the game.
	It is called after a certain amount of time has started, so the splash
	screen can be seen.
	--]]

	-- calls the transition function after waiting on the splash screen for a period of time
	local function transition (_event) composer.gotoScene("source.menu", { effect = "fade", time = 1000 }) end
	timer.performWithDelay(1500, transition)
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
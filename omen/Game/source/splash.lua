--[[
Splash script contains all the code for the splash screen,
wrapped-up inside the splash scene. This scene is the first in
the application, when the game loads it boots directly into the
splash screen for a short period. After a while it will load
the menu up.
--]]

require("source.constant")
require("source.letterbox")

local composer = require("composer")
composer.recycleOnSceneChange = true
local scene = composer.newScene()

function scene:create (_event)
	local background = display.newImageRect("assets/images/titlescreen.png", 320, 240)
	background.x = Constant.CONTENT_CENTER_X; background.y = Constant.CONTENT_CENTER_Y
	self.view:insert(background)

	if Letterbox.bleed.left ~= nil then self.view:insert(Letterbox.bleed.left) end
	if Letterbox.bleed.right ~= nil then self.view:insert(Letterbox.bleed.right) end
	if Letterbox.bleed.top ~= nil then self.view:insert(Letterbox.bleed.top) end
	if Letterbox.bleed.bottom ~= nil then self.view:insert(Letterbox.bleed.bottom) end

	local function transition (_event)
		composer.gotoScene("source.menu", { effect = "fade", time = 1000 })
	end
	timer.performWithDelay(1500, transition)
end

function scene:destroy (_event)
	display.remove(self.view)
end

scene:addEventListener("create", scene)
scene:addEventListener("destroy", scene)

return scene
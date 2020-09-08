--[[
Restart script is used as an intermediary scene so that the
game can transition into itself on restart. As it is not
possible for a corona composer to goto the same scene it is
currently in.
--]]

local composer = require("composer")
composer.recycleOnSceneChange = true
local scene = composer.newScene()

function scene:create (_event)
	function restart (_event) composer.gotoScene("source.game", { effect = "fade", time = 1000 }) end
	timer.performWithDelay(1000, restart)
end

function scene:destroy (_event)
	display.remove(self.view)
end

scene:addEventListener("create", scene)
scene:addEventListener("destroy", scene)

return scene
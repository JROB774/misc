--[[
Restart script is used as an intermediary scene so that the game can
transition into itself on restart. As it is not possible for a Corona
composer to goto the same scene it is currently in.
--]]

-- creates the create scene via a scene composer
local composer = require("composer")
composer.recycleOnSceneChange = true -- clears the scene from memory after use
local scene = composer.newScene()

--[[
This function is called when the restart scene is created, it is simply
used to setup the callback that will be called once the timer completes.
This will return the composer to the game scene it just came from.
--]]

function scene:create (_event)
	--[[
	The restart function makes the composer go back to the game scene. This
	function is called on a timer as Corona will stop working if a goto() is
	called whilst still transitioning into a scene from another.
	--]]

	-- calls the restart function after the loading transition is complete (breaks without the timer)
	local function restart (_event) composer.gotoScene("source.game", { effect = "fade", time = 1000 }) end
	timer.performWithDelay(1000, restart)
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
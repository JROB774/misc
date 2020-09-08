require("data.scripts.state")

local TCE = require("data.scripts.tce.tce")

function Initialize ()
	-- Load all of our assets.
	feelies.texture = TCE.LoadTextureFromFile("feelies.png");
	feelies.animation = TCE.LoadAnimationFromFile("feelies.anim");

	planks.texture = TCE.LoadTextureFromFile("planks.png");
end

function Update (_dt)
	-- Scroll the planks to the left.
	planks.x = planks.x - (40.0 * _dt)
	if planks.x <= -320.0 then
		planks.x = planks.x + 320.0
	end

	-- Move the feelie in a wavy pattern.
	feelies.x = (320.0 - 100.0) / 2.0
	feelies.y = (200.0 - 100.0) / 2.0

	 feelies.x = feelies.x + math.sin(feelies.counter) * 100.0
	feelies.y = feelies.y + math.sin(feelies.counter / 2.0) * 30.0

	feelies.counter = feelies.counter + (5.0 * _dt)
end

function Render ()
	-- Render the planks and feelie to the screen.
	TCE.RenderTexture(planks.texture, planks.x, planks.y)
	TCE.RenderTexture(planks.texture, planks.x + 320.0, planks.y)

	TCE.RenderTextureAnimated(feelies.texture, feelies.animation, feelies.x, feelies.y)
end

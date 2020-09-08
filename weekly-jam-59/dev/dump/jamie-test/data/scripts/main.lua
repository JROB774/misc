local TCE = require("data.scripts.tce.tce")
require("data.scripts.state")

function Initialize ()
	final_boss.image = TCE.LoadTextureFromFile("finalboss.png")
	final_boss.vector.x = 100;
end

function Update (_dt)
	if final_boss.bounds.x + final_boss.bounds.w > TCE.GetViewportWidth() then
		final_boss.vector.x = -100
		final_boss.change_colour()
		TCE.SetTextureColor(final_boss.image, final_boss.colour)
	end

	if final_boss.bounds.x < 0 then 
		final_boss.vector.x = 100
		final_boss.change_colour()
		TCE.SetTextureColor(final_boss.image, final_boss.colour)
	end

	final_boss.bounds.x = final_boss.bounds.x + (final_boss.vector.x * _dt)
end

function Render ()

	TCE.RenderTexture(final_boss.image, {x = final_boss.bounds.x, y = final_boss.bounds.y})
end

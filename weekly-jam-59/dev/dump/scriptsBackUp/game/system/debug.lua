local TCE = require("data.scripts.tce.tce")

Debug = {}

Debug.ShowInfo = true
Debug.RenderInfo = function ()
	-- Render debug information that we would like to know about.
	if Debug.ShowInfo then
		TCE.RenderText(fnt["debug"], string.format("FPS: %g", TCE.GetFPS()), { x = 2, y = 0 })
		TCE.RenderText(fnt["debug"], string.format("Player Pos: %d, %d", player.pos.x, player.pos.y), { x = 2, y = 8 })
		TCE.RenderText(fnt["debug"], string.format("Player HP:  %f", player.health), { x = 2, y = 16 })
	end
end

local TCE = require("data.scripts.tce.tce")

Debug = {}

Debug.ShowTextInfo     = false
Debug.ShowTileInfo     = false
Debug.ShowCollideInfo = false

Debug.RenderTextInfo = function ()
	-- Render debug information that we would like to know about.
	if Debug.ShowTextInfo then
		TCE.RenderRectFilled(0, 0, TCE.GetViewportWidth(), 48, "0x0000003F")

		-- Caching these values here to make the code more readable.
		local sortIterations         = Entity.debugSortIterations
		local renderListLength       = Entity.debugListLength
		local activeRenderListLength = Entity.debugActiveListLength
		local tileListLengtb         = TCE.TableLength(tileList)
		local reserveTileListLength  = TCE.TableLength(reserveTileList)

		TCE.RenderShadowedText(fnt["debug"], string.format("Framerate:      %g",       TCE.Round(TCE.GetFPS())),                                  "0xFFFFFFFF", "0x000000FF", 1, 1, { x = 2, y =  0 })
		TCE.RenderShadowedText(fnt["debug"], string.format("Player Pos:     %g, %g",   TCE.Round(player.pos.x), TCE.Round(player.pos.y)),         "0xFFFFFFFF", "0x000000FF", 1, 1, { x = 2, y =  8 })
		TCE.RenderShadowedText(fnt["debug"], string.format("Player HP:      %g",       TCE.Round(player.stats.health)),                           "0xFFFFFFFF", "0x000000FF", 1, 1, { x = 2, y = 16 })
		TCE.RenderShadowedText(fnt["debug"], string.format("Player Max HP:  %g",       TCE.Round(player.stats.maxHealth)),                        "0xFFFFFFFF", "0x000000FF", 1, 1, { x = 2, y = 24 })
		TCE.RenderShadowedText(fnt["debug"], string.format("Entity Sort:    %g %g %g", sortIterations, renderListLength, activeRenderListLength), "0xFFFFFFFF", "0x000000FF", 1, 1, { x = 2, y = 32 })
		TCE.RenderShadowedText(fnt["debug"], string.format("Tile Lists:     %g %g",    tileListLengtb, reserveTileListLength),                    "0xFFFFFFFF", "0x000000FF", 1, 1, { x = 2, y = 40 })
	end
end

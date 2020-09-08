local TCE = require("data.scripts.tce.tce")

Cursor = {}
Cursor.pos = { x = 0, y = 0 }

function Cursor.initialize ()
	-- We don't want to see the system cursor because we have our own.
	TCE.ShowCursor(false)
end

function Cursor.update (dt)
	Cursor.pos.x, Cursor.pos.y = TCE.GetMousePosition()
end

function Cursor.render ()
	local xRender = Cursor.pos.x - (TCE.GetTextureWidth(tex["cursor"]) / 2)
	local yRender = Cursor.pos.y - (TCE.GetTextureHeight(tex["cursor"]) / 2)

	TCE.RenderTexture(tex["cursor"], { x = xRender, y = yRender })
end

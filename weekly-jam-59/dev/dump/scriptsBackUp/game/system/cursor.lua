local TCE = require("data.scripts.tce.tce")

Cursor = {}

Cursor.xPos    = 0
Cursor.yPos    = 0
Cursor.texture = nil

function Cursor:init ()
	self.texture = TCE.LoadTextureFromFile("cursor.png")
	-- We don't want to see the system cursor because we have our own.
	TCE.ShowCursor(false)
end

function Cursor:update (dt)
	self.xPos, self.yPos = TCE.GetMousePosition()
end

function Cursor:render ()
	local xRender = self.xPos - (TCE.GetTextureWidth(self.texture) / 2)
	local yRender = self.yPos - (TCE.GetTextureHeight(self.texture) / 2)

	TCE.RenderTexture(Cursor.texture, { x=xRender, y=yRender })
end

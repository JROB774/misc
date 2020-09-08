local TCE = require("data.scripts.tce.tce")

require("data.scripts.state")

function Initialize ()
end

function Update (_dt)
	a.x, a.y = TCE.GetMousePosition()

	a.x = a.x - (a.w / 2)
	a.y = a.y - (a.h / 2)

	result = TCE.BoxAndBoxCollision(a, b)
end

function Render ()
	if result then
		TCE.RenderRectFilled(a.x, a.y, a.w, a.h, "0xFF0000FF")
		TCE.RenderRectFilled(b.x, b.y, b.w, b.h, "0x0000FFFF")
	else
		TCE.RenderRectOutline(a.x, a.y, a.w, a.h, "0xFF0000FF")
		TCE.RenderRectOutline(b.x, b.y, b.w, b.h, "0x0000FFFF")
	end
end

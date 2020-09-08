--[[
Letterbox script contains all the information regarding the screen
bleed on the current device. It is initialised at the start of the
application and creates four border images that can be rendered
in this bleed area to avoid black bars.
--]]

require("source.constant")

-- the global letterbox namespace
Letterbox = {}
Letterbox.__index = Letterbox

-- stores the bleed information for letterboxing
Letterbox.bleed = {}

-- sets up the letter boxing information
function Letterbox.init ()
	local horzbleed = math.floor(Constant.SCREEN_WIDTH - Constant.CONTENT_WIDTH)
	local vertbleed = math.floor(Constant.SCREEN_HEIGHT - Constant.CONTENT_HEIGHT)

	if horzbleed > 0 then
		Letterbox.bleed.left = display.newImageRect("assets/images/border.png", horzbleed / 2, Constant.SCREEN_HEIGHT)
		Letterbox.bleed.left.x = -(horzbleed / 4); Letterbox.bleed.left.y = Constant.CONTENT_CENTER_Y
		Letterbox.bleed.left:setFillColor(0.2, 0.2, 0.2)
		Letterbox.bleed.right = display.newImageRect("assets/images/border.png", horzbleed / 2, Constant.SCREEN_HEIGHT)
		Letterbox.bleed.right.x = Constant.CONTENT_WIDTH + (horzbleed / 4); Letterbox.bleed.right.y = Constant.CONTENT_CENTER_Y
		Letterbox.bleed.right:setFillColor(0.2, 0.2, 0.2)
	end

	if vertbleed > 0 then
		Letterbox.bleed.top = display.newImageRect("assets/images/border.png", Constant.CONTENT_WIDTH, vertbleed / 2)
		Letterbox.bleed.top.x = Constant.CONTENT_CENTER_X; Letterbox.bleed.top.y = -(vertbleed / 4)
		Letterbox.bleed.top:setFillColor(0.2, 0.2, 0.2)
		Letterbox.bleed.bottom = display.newImageRect("assets/images/border.png", Constant.CONTENT_WIDTH, vertbleed / 2)
		Letterbox.bleed.bottom.x = Constant.CONTENT_CENTER_X; Letterbox.bleed.bottom.y = Contant.CONTENT_HEIGHT + (vertbleed / 4) - 1
		Letterbox.bleed.bottom:setFillColor(0.2, 0.2, 0.2)
	end
end
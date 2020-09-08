local TCE = require("data.scripts.tce.tce")

require("data.scripts.game.system.game")

function initialize ()
	Map.loadZone("startland.png")

	Cursor:init()
	player = Player.new(38 * TILE_WIDTH, 54 * TILE_HEIGHT, 30, 1, 90.0, 0)
	--Defender.new(100, 120, 0 ) -- @TEMP
	TCE.SetShadowColor(TCE.MakeColor(40, 40, 255, 255))
	TCE.SetLightColor(TCE.MakeColor(180, 160, 220, 255))
end

function update (dt)
	Cursor:update(dt)

	-- We do BeginCamera because all logic in here should consider the camera.
	TCE.BeginCamera()
	player:update(dt)
	for key in pairs(entityList) do
		if entityList[key].updateOn == true then
			entityList[key]:update(dt, player)
		end
	end
	TCE.EndCamera()

	-- Toggle debug information on and off.
	if TCE.KeyPressed(TCE.Keycodes.KEY_F1) then
		Debug.ShowInfo = not Debug.ShowInfo
	end
end

function render ()
	-- We do BeginCamera because all rendering in here should consider the camera.
	TCE.BeginCamera()
	for key in pairs(renderList) do
		if renderList[key].renderOn == true then
			renderList[key]:render()
		end
	end
	player:render()
	TCE.EndCamera()

	TCE.RenderLighting()
	Cursor:render()

	Debug.RenderInfo()
end

local TCE = require("data.scripts.tce.tce")
require("data.scripts.game.system.game")

function initialize ()
	TCE.SetShadowColor(TCE.MakeColor(40,  40, 255, 255))
	TCE.SetLightColor(TCE.MakeColor(180, 160, 220, 255))

	TCE.TurnLightingOn(false)

	Map.loadZone(Start.ZONE)
	player = Player.new(Start.PLAYER_POS)

	Cursor.initialize()
end

function update (dt)
	-- Toggle various bits of debug information on and off.
	if TCE.KeyPressed(TCE.Keycodes.KEY_F1) then Debug.ShowTextInfo    = not Debug.ShowTextInfo    end
	if TCE.KeyPressed(TCE.Keycodes.KEY_F2) then Debug.ShowTileInfo    = not Debug.ShowTileInfo    end
	if TCE.KeyPressed(TCE.Keycodes.KEY_F3) then Debug.ShowCollideInfo = not Debug.ShowCollideInfo end

	Cursor.update(dt)

	-- Determine the current room the camera should be looking at.
	cameraX = math.floor((player.pos.x + (player.width / 2)) / TCE.GetViewportWidth()) * TCE.GetViewportWidth()
	cameraY = math.floor((player.pos.y + (player.height / 2)) / TCE.GetViewportHeight()) * TCE.GetViewportHeight()

	oldRoom.x     = currentRoom.x
	oldRoom.y     = currentRoom.y
	currentRoom.x = cameraX / TCE.GetViewportWidth()
	currentRoom.y = cameraY / TCE.GetViewportHeight()

	TCE.SetCameraPosition(cameraX, cameraY)

	-- If we are in a new room then we want to update what tiles we are handling.
	if currentRoom.x ~= oldRoom.x or currentRoom.y ~= oldRoom.y then
		loadEnemies()
		for key in pairs(reserveTileList) do
			reserveTileList[key]:update(dt, player)
		end
	end

	-- We do BeginCamera because all logic in here should consider the camera.
	TCE.BeginCamera()
	player:update(dt)
	for key in pairs(entityList) do
		if entityList[key].flags.updateOn == true then
			entityList[key]:update(dt, player)
		end
	end
	TCE.EndCamera()
end

function render ()
	-- We do BeginCamera because all renders in here should consider the camera.
	TCE.BeginCamera()
	sortedRenderList = {}
	for v in pairs(tileList) do
		table.insert(sortedRenderList, tileList[v])
  end
  for v in pairs(renderList) do
  	table.insert(sortedRenderList, renderList[v])
  end
  Entity.sortRenderList(sortedRenderList)
	for key in pairs(sortedRenderList) do
		if sortedRenderList[key].flags.renderOn == true then
			sortedRenderList[key]:render()
			-- Render debug information regarding entity colliders.
  		if Debug.ShowCollideInfo and sortedRenderList[key].flags.solid then
  			TCE.RenderRectFilled(sortedRenderList[key].pos.x, sortedRenderList[key].pos.y,
  				sortedRenderList[key].width, sortedRenderList[key].height, "0xFF0000AF")
  		end
		end
	end
	TCE.EndCamera()

	TCE.RenderLighting()

	Health.render()
	Cursor.render()

	Debug.RenderTextInfo()
end

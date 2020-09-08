local TCE = require("data.scripts.tce.tce")

TILE_WIDTH  = 20
TILE_HEIGHT = 20

cameraY = 0
cameraX = 0

entityList      = {}
renderList      = {}
reserveTileList = {}
tileList        = {}
player          = {}

function resetLists ()
	entityList = {}
	renderList = {}
	tileList   = {}
end

local TCE = require("data.scripts.tce.tce")


require("data.scripts.game.system.entity")
require("data.scripts.game.enemy.slime")

TILE_WIDTH  = 20
TILE_HEIGHT = 20

-- @TODO: GIVE THESE ASSET TABLES BETTER NAMES AT SOME POINT
tex =
{
	["player"] = TCE.LoadTextureFromFile("player.png");
	["spikeblock"] = TCE.LoadTextureFromFile("spike_block.png");
	["slime"] = TCE.LoadTextureFromFile("slime.png");
	["madRabbit"] = TCE.LoadTextureFromFile("madRabbit.png");
	["defender"] = TCE.LoadTextureFromFile("defender.png");
	["heartBullet"] = TCE.LoadTextureFromFile("bullets/heartBullet.png");
	["greatSwordSlash"] = TCE.LoadTextureFromFile("bullets/greatSwordSlash.png");
	["terrain"] = TCE.LoadTextureFromFile("terrain.png");
}
anm =
{
	["player_idle"] = TCE.LoadAnimationFromFile("player_idle.anim");
	["player_walk"] = TCE.LoadAnimationFromFile("player_walk.anim");
}
fnt =
{
	["console"] = TCE.LoadFontFromFile("console.font");
	["debug"] = TCE.LoadFontFromFile("debug.font");
}
snd =
{
	["player_walk"] = TCE.LoadSoundFromFile("player_walk.wav");
	["player_walk_stone"] = TCE.LoadSoundFromFile("player_walk_stone.wav");
}

entityList = {}
renderList = {}
tileList   = {}
player     = {}

function resetLists()
	entityList = {}
	renderList = {}
	tileList   = {}
end

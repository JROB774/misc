local TCE = require("data.scripts.tce.tce")

tex =
{
	["player"]                = TCE.LoadTextureFromFile("player.png");
	["tileset"]               = TCE.LoadTextureFromFile("tileset.png");
	["pickups"]               = TCE.LoadTextureFromFile("pickups.png");
	["items"]                 = TCE.LoadTextureFromFile("items.png");
	["scrunt"]                = TCE.LoadTextureFromFile("entities/scrunt.png");
	["scrimbat"]		          = TCE.LoadTextureFromFile("entities/scrimbat.png");
	["digglydoo"]             = TCE.LoadTextureFromFile("entities/digglydoo.png");
	["spikeblock"]            = TCE.LoadTextureFromFile("entities/spike_block.png");
	["slime"]                 = TCE.LoadTextureFromFile("entities/slime.png");
	["madRabbit"]             = TCE.LoadTextureFromFile("entities/madRabbit.png");
	["defender"]              = TCE.LoadTextureFromFile("entities/defender.png");
	["love"]                  = TCE.LoadTextureFromFile("bullets/love.png");
	["greatswordSlash"]       = TCE.LoadTextureFromFile("bullets/greatSwordSlash.png");
	["bone"]                  = TCE.LoadTextureFromFile("bullets/bone.png");
	["teleporter"]            = TCE.LoadTextureFromFile("teleporter.png");
	["snoteleks"]             = TCE.LoadTextureFromFile("snoteleks.png");
	["health"]                = TCE.LoadTextureFromFile("gui/health.png");
	["cursor"]                = TCE.LoadTextureFromFile("gui/cursor.png");
	-- @TEMP:
	["healthSmall"]	          = TCE.LoadTextureFromFile("healthSmall.png");
	["speed"]                 = TCE.LoadTextureFromFile("speed.png");
}
anm =
{
	["player_idle"]           = TCE.LoadAnimationFromFile("player_idle.anim");
	["player_walk"]           = TCE.LoadAnimationFromFile("player_walk.anim");
	["health_full"]           = TCE.LoadAnimationFromFile("gui_health_full.anim");
	["health_half"]           = TCE.LoadAnimationFromFile("gui_health_half.anim");
	["health_none"]           = TCE.LoadAnimationFromFile("gui_health_none.anim");
	["heart_full"]            = TCE.LoadAnimationFromFile("pickup_heartfull.anim");
	["heart_half"]            = TCE.LoadAnimationFromFile("pickup_hearthalf.anim");
	["lifeup"]                = TCE.LoadAnimationFromFile("item_lifeup.anim");
	["digglydoo_wait"]        = TCE.LoadAnimationFromFile("entity_digglydoo_wait.anim");
	["digglydoo_shoot"]       = TCE.LoadAnimationFromFile("entity_digglydoo_shoot.anim");
	["digglydoo_move"]        = TCE.LoadAnimationFromFile("entity_digglydoo_move.anim");
	["love"]                  = TCE.LoadAnimationFromFile("bullet_love.anim");
	["love_death"]            = TCE.LoadAnimationFromFile("bullet_love_death.anim");
}
fnt =
{
	["console"]               = TCE.LoadFontFromFile("console.font");
	["debug"]                 = TCE.LoadFontFromFile("debug.font");
}
snd =
{
	-- ["player_walk"]           = TCE.LoadSoundFromFile("player_walk.wav");
	-- ["player_walk_stone"]     = TCE.LoadSoundFromFile("player_walk_stone.wav");
}

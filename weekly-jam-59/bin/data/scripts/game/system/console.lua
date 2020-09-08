--[[----------------------------------------------------------------------------

 This file contains all of the command that can be used by the engine's
 debug console. Simply define new functions for the console module and
 then they will be usable by the debug console (activated by pressing `).

----------------------------------------------------------------------------]]--

local TCE = require("data.scripts.tce.tce")

Console = {}
Console.HelpList =
{
	{ name = "help",        params = "\"[command]\"",    desc = "Prints list of commands or specific command info."               };
	{ name = "reload",      params = "",                 desc = "Reloads the entire Lua state."                                   };
	{ name = "clear",       params = "",                 desc = "Clears the console history of text."                             };
	{ name = "exit",        params = "",                 desc = "Exits the game."                                                 };
	{ name = "print",       params = "<text>",           desc = "Prints a string or Lua code that can evaluate to a string."      };
	{ name = "teleport",    params = "<x> <y>",          desc = "Teleports the player to the provided X,Y coordinates."           };
	{ name = "spawn",       params = "<id>",             desc = "Spawns a tile with the specified ID at the current position."    };
	{ name = "speed",       params = "<speed>",          desc = "Sets the player's speed to the specified value."                 };
	{ name = "viewport",    params = "<width> <height>", desc = "Sets the size of the game viewport (the visible camera area)."   };
	{ name = "health",      params = "<health>",         desc = "Sets the player's health, where 1 is a half heart."              };
	{ name = "max_health",  params = "<max_health>",     desc = "Sets the player's max health (only multiples of 2 are allowed)." };
}

function Console.help (command)
	if command ~= nil then
		for k,v in pairs(Console.HelpList) do
			if v.name == command then
				if (v.params ~= "") then TCE.ConsolePrint(string.format("%s %s - %s", v.name, v.params, v.desc))
				else TCE.ConsolePrint(string.format("%s - %s", v.name, v.desc)) end
				return
			end
		end
		TCE.ConsolePrint(string.format("Command '%s' not found!", command))
	else
		for k,v in pairs(Console.HelpList) do
			TCE.ConsolePrint(string.format("%s - %s", v.name, v.desc))
		end
	end
end

function Console.reload ()
	TCE.Private.ReloadGame()
end

function Console.clear ()
	TCE.Private.ClearConsole()
end

function Console.exit ()
	TCE.Exit()
end

function Console.print (text)
	TCE.ConsolePrint(text)
end

function Console.teleport (x, y)
	player.pos.x = x
	player.pos.y = y
end

function Console.spawn (id)
	tile_x = math.floor((player.pos.x + (player.width / 2)) / TILE_WIDTH) * TILE_WIDTH
	tile_y = math.floor((player.pos.y + (player.height / 2)) / TILE_HEIGHT) * TILE_HEIGHT

	id_number = tonumber(id)

	Tile.new({ x = tile_x, y = tile_y }, blocks[id_number].texture, blocks[id_number].tileType, 0, 0)
end

function Console.speed (speed)
	player.stats.maxSpeed = speed
end

function Console.viewport (width, height)
	TCE.Private.SetViewport(width, height)
end

function Console.health (health)
	player.stats.health = health
	if player.stats.health > player.stats.maxHealth then
		player.stats.health = player.stats.maxHealth
	end
end

function Console.max_health (maxHealth)
	player.stats.maxHealth = (maxHealth % 2 == 0) and maxHealth or maxHealth + 1
	if player.stats.health > player.stats.maxHealth then
		player.stats.health = player.stats.maxHealth
	end
end

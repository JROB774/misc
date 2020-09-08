--[[----------------------------------------------------------------------------

 This file contains all of the command that can be used by the engine's
 debug console. Simply define new functions for the console module and
 then they will be usable by the debug console (activated by pressing `).

----------------------------------------------------------------------------]]--

local TCE = require("data.scripts.tce.tce")

Console = {}
Console.HelpList =
{
	{ name = "help",   params = "\"[command]\"", desc = "Prints list of commands or specific command info."          },
	{ name = "reload", params = "",              desc = "Reloads the entire Lua state."                              },
	{ name = "clear",  params = "",              desc = "Clears the console history of text."                        },
	{ name = "exit",   params = "",              desc = "Exits the game."                                            },
	{ name = "print",  params = "<text>",        desc = "Prints a string or Lua code that can evaluate to a string." }
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

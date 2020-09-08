local TCE = require("data.scripts.tce.tce")

currentZone = Start.ZONE
oldRoom     = { x = 0, y = 0 }
currentRoom = { x = 0, y = 0 }

function loadEnemies()
	--room = currentZone .. "." .. math.floor(pos.x / TCE.GetViewportWidth()) .. "." .. math.floor(pos.y / TCE.GetViewportWidth())
	room = currentZone .. "-" .. currentRoom.x .. "-" .. currentRoom.y
	print(room)
	for k in pairs(roomEntities) do
		if(k == room) then
			roomEntities[room]()
		end
	end
end

roomEntities = {

	["debug_01-2.0-2.0"] = function()
		--Slime.new ({ x = 44, y =  28 })a
		Digglydoo.new ({ x = 44, y = 28 })
		Scrimbat.new ({ x = 25, y = 8 })
		--Scrunt.new ({ x = 575, y = 350 })
	end ;

	["debug_01-0.0-0.0"] = function()
		--Defender.new ({ x = TCE.GetViewportWidth() * currentRoom.x + 40, y = TCE.GetViewportHeight() * currentRoom.y + 60 })
		--Transporter.new ({ x = 10, y = 20 }, "testland", {x = 19, y = 6})
		Snoteleks.new({x = 5, y = 8})
		--Defender.new({x=5, y = 5})
		--Scrimbat.new ({ x = 380, y = 100 })
		--Digglydoo.new ({ x = 380, y = 60 })
	end;

	["debug_01-3.0-3.0"] = function ()
		SpikeBlock.new({x = 54, y =41}, {x = 0, y = -1})
		Scrunt.new({x = 60, y =36})
	end;

	["debug_01-3.0-2.0"] = function()
		Snoteleks.new({x = 53, y = 28})
		Snoteleks.new({x = 60, y = 28})
		SpikeBlock.new({x = 56, y =32}, {x = 0, y = 1})
	end;
}

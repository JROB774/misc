final_boss =
{
	image = 0,

	bounds = {x = 0, y = 0, w = 389, h = 0},
	vector = {x = 0, y = 0},

	colour = "0xFFFFFFFF",

	change_colour = function()
		colour = final_boss.colour
		if colour == "0xFFFFFFFF" then
			colour = "0xFF0000FF"
		elseif colour == "0xFF0000FF" then
			colour = "0x00FF00FF"
		elseif colour == "0x00FF00FF" then
			colour = "0x0000FFFF"
		else
			colour = "0xFFFFFFFF"
		end
		final_boss.colour = colour
	end
}
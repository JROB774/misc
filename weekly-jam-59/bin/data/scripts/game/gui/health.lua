local TCE = require("data.scripts.tce.tce")

Health = {}
Health.width = 20

Health.render = function ()
	-- Render the player's current health UI.
	local healthX = 2
	local healthY = 0

	local maxHealthInHearts = (player.stats.maxHealth / 2)
	local healthInHalfHearts = player.stats.health

	for i = maxHealthInHearts, 1, -1 do
		-- Determine which sprite to use when drawing the current heart container.
		local health_container_anim = "health_none"
		if healthInHalfHearts == 1 then health_container_anim = "health_half"
		elseif healthInHalfHearts > 1 then health_container_anim = "health_full" end

		TCE.RenderAnimatedTexture(tex["health"], anm[health_container_anim], { x = healthX, y = healthY })
		healthInHalfHearts = healthInHalfHearts - 2

		healthX = healthX + Health.width
	end
end

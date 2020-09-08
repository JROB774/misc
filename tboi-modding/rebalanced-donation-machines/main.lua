-- *****************************************************************************
-- Main file for the 'Rebalanced Donation Machines' mod for TBOI: AB+.
-- Public Domain / Unlicense <www.unlicense.org>
-- Authored by Joshua Robertson
--
-- *****************************************************************************

local mod  = RegisterMod("Rebalanced Donation Machines", 1)
local game = Game()

-- We simply reset the state of the donation machine
-- when the player moves onwards to the next level.
mod:AddCallback(ModCallbacks.MC_POST_NEW_LEVEL, function ()
	if game:GetStateFlag(GameStateFlag.STATE_DONATION_SLOT_JAMMED) then
		game:SetStateFlag(GameStateFlag.STATE_DONATION_SLOT_JAMMED, false)
	end
end)

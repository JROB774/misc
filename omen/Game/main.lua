--[[
Main script is the entry-point for the application, through Corona.
Firstly, default parameters and basic application initialisation is
done; afterwards the game is entered through the splash scene.
--]]

system.activate("multitouch")

display.setDefault("magTextureFilter", "nearest")
display.setDefault("minTextureFilter", "nearest")

display.setStatusBar(display.HiddenStatusBar)

require("source.letterbox")
Letterbox.init()

local composer = require("composer")
composer.gotoScene("source.splash")
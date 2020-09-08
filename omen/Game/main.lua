--[[
Main script is the entry-point for the application, through Corona.
Firstly, default parameters and basic application initialisation is
done; afterwards the game is entered through the splash scene.
--]]

-- activates multitouch functionality for mobile devices
system.activate("multitouch")

-- makes sure all textures are scaled without filtering, maintains pixels
display.setDefault("magTextureFilter", "nearest")
display.setDefault("minTextureFilter", "nearest")

-- hides the status bar whilst being played on mobiles
display.setStatusBar(display.HiddenStatusBar)

-- inititalises the code that creates the bordering letter box images
require("source.letterbox")
Letterbox.init()

-- enters into the splash screen via a game scene composer
local composer = require("composer")
composer.gotoScene("source.splash")
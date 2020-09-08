
-- Miscellaneous --------------------------------------------------------------
require("data.scripts.game.system.zIndex")
require("data.scripts.game.system.startup")
-------------------------------------------------------------------------------

-- System ---------------------------------------------------------------------
require("data.scripts.game.system.globals")
require("data.scripts.game.system.assets")
require("data.scripts.game.system.collision")
require("data.scripts.game.system.rooms")
-------------------------------------------------------------------------------

-- Entities -------------------------------------------------------------------
require("data.scripts.game.entities.entity")
require("data.scripts.game.entities.tile")
require("data.scripts.game.entities.player")
require("data.scripts.game.entities.enemies.scrunt")
require("data.scripts.game.entities.enemies.scrimbat")
require("data.scripts.game.entities.enemies.digglydoo")
require("data.scripts.game.entities.enemies.defender")
require("data.scripts.game.entities.enemies.slime")
require("data.scripts.game.entities.enemies.spikeBlock")
require("data.scripts.game.entities.enemies.snoteleks")
require("data.scripts.game.entities.bullets.bullet")
require("data.scripts.game.entities.bullets.bone")
require("data.scripts.game.entities.bullets.heart")
require("data.scripts.game.entities.other.spawnItem")
require("data.scripts.game.entities.other.transporter")
-------------------------------------------------------------------------------

-- GUI ------------------------------------------------------------------------
require("data.scripts.game.gui.cursor")
require("data.scripts.game.gui.health")
-------------------------------------------------------------------------------

-- Map ------------------------------------------------------------------------
require("data.scripts.game.system.map")
-------------------------------------------------------------------------------

-- Debugging ------------------------------------------------------------------
require("data.scripts.game.system.console")
require("data.scripts.game.system.debug")
-------------------------------------------------------------------------------

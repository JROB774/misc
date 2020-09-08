--[[
Constant script contains many global constants for the application.
They are all stored here as a way to avoid the frequent use of magic
numbers throughout the code (more readable).
--]]

-- includes functionality from other scripts
require("source.utility")

-- the global constant namespace is created
Constant = {}
Constant.__index = Constant

-- defines all the constants
Constant.SCREEN_WIDTH = display.actualContentWidth
Constant.SCREEN_HEIGHT = display.actualContentHeight
Constant.CONTENT_WIDTH = display.contentWidth
Constant.CONTENT_HEIGHT = display.contentHeight
Constant.CONTENT_CENTER_X = display.contentCenterX
Constant.CONTENT_CENTER_Y = display.contentCenterY
Constant.TILE_WIDTH = 32
Constant.TILE_HEIGHT = 48
Constant.TILE_DISTANCE_SHADOW = 500
Constant.TILE_SIDE_SHADOW = 0.15
Constant.TILE_SOLID = 1
Constant.TILE_EMPTY = 0
Constant.PLAYER_FOV = 60.0
Constant.PLAYER_HEIGHT = 24
Constant.PLAYER_MOVE_SPEED = 4.5
Constant.PLAYER_TURN_SPEED = 14.5
Constant.PLAYER_START_X = 256
Constant.PLAYER_START_Y = 256
Constant.PLAYER_START_ANGLE = 90.0
Constant.PROJPLANE_WIDTH = 320
Constant.PROJPLANE_HEIGHT = 240
Constant.PROJPLANE_DISTANCE = (Constant.PROJPLANE_WIDTH / 2) / math.tan(math.rad(Constant.PLAYER_FOV / 2))
Constant.RAY_LENGTH = 14
Constant.RAY_ANGLE = Constant.PLAYER_FOV / Constant.PROJPLANE_WIDTH
Constant.RAY_OUT_OF_BOUNDS = 99999
Constant.MENU_STATE_MAIN = 0
Constant.MENU_STATE_SETTINGS = 1
Constant.MENU_STATE_HELP = 2
Constant.MAX_AUDIO_CHANNELS = 32
Constant.MUSIC_TOTAL = 3
Constant.NPC_FAMILIAR_HEALTH = 2
Constant.NPC_FAMILIAR_BOUNDS = 32
Constant.NPC_FAMILIAR_MOVE_SPEED = 1.5
Constant.NPC_DISTANCE_SHADOW = 500
Constant.SPAWNER_TOTAL = 3
Constant.SPAWNER_POS_X = {}
Constant.SPAWNER_POS_X[1] = 256
Constant.SPAWNER_POS_X[2] = 64
Constant.SPAWNER_POS_X[3] = 448
Constant.SPAWNER_POS_Y = {}
Constant.SPAWNER_POS_Y[1] = 64
Constant.SPAWNER_POS_Y[2] = 256
Constant.SPAWNER_POS_Y[3] = 256
Constant.SPAWN_TIME_START_1 = 90
Constant.SPAWN_TIME_START_2 = 180
Constant.SPAWN_TIME_START_3 = 1260
Constant.SPAWN_TIME_MIN = 35
Constant.SPAWN_TIME_MAX = 125
Constant.SCORE_HIT = 5
Constant.SCORE_KILL = 10
Constant.AIM_EASING = 3.0

-- makes the constant table immutable (so it cannot be changed)
Constant = Utility.immutable(Constant)